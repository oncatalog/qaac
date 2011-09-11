#ifndef _COMPOSITE_H
#define _COMPOSITE_H

#include "shared_ptr.h"
#include "iointer.h"

class CompositeSource: public ISource, public ITagParser {
    typedef x::shared_ptr<ISource> source_t;
    std::vector<source_t> m_sources;
    SampleFormat m_format;
    size_t m_curpos;
    std::map<uint32_t, std::wstring> m_tags;
public:
    CompositeSource() : m_curpos(0) {}
    const std::vector<uint32_t> *getChannelMap() const { return 0; }
    const SampleFormat &getSampleFormat() const { return m_format; }
    const std::map<uint32_t, std::wstring> &getTags() const { return m_tags; }
    const std::vector<std::pair<std::wstring, int64_t> > *getChapters() const
    {
	return 0;
    }
    void setTags(const std::map<uint32_t, std::wstring> &tags)
    {
	m_tags = tags;
    }
    void addSource(const x::shared_ptr<ISource> &src)
    {
	if (!m_sources.size())
	    m_format = src->getSampleFormat();
	else if (m_format != src->getSampleFormat())
	    throw std::runtime_error(
		    "CompositeSource: can't compose different sample format");
	m_sources.push_back(src);
    }
    uint64_t length() const
    {
	uint64_t len = 0;
	for (size_t i = 0; i < m_sources.size(); ++i)
	    len += m_sources[i]->length();
	return len;
    }
    size_t readSamples(void *buffer, size_t nsamples)
    {
	if (m_curpos == m_sources.size())
	    return 0;
	size_t rc = m_sources[m_curpos]->readSamples(buffer, nsamples);
	if (rc == nsamples)
	    return rc;
	if (rc == 0) {
	    ++m_curpos;
	    return readSamples(buffer, nsamples);
	}
	return rc + readSamples(
	    reinterpret_cast<char*>(buffer) + rc * m_format.bytesPerFrame(),
	    nsamples - rc);
    }
    void setRange(int64_t start=0, int64_t length=-1)
    {
	throw std::runtime_error("CompositeSource::setRange: not implemented");
    }
};

#endif
