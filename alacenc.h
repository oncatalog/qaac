#ifndef ALACENC_H
#define ALACENC_H

#include "iointer.h"
#include "iencoder.h"
#include <GNUCompatibility/stdint.h>
#include <ALACEncoder.h>

class ALACEncoderX: public IEncoder, public IEncoderStat {
    union ASBD {
	AudioStreamBasicDescription asbd;
	AudioFormatDescription afd;
    };
    x::shared_ptr<ISource> m_src;
    x::shared_ptr<ISink> m_sink;
    x::shared_ptr<ALACEncoder> m_encoder;
    std::vector<uint8_t> m_input_buffer;
    std::vector<uint8_t> m_output_buffer;
    ASBD m_input_desc, m_output_desc;
    EncoderStat m_stat;
public:
    ALACEncoderX(const AudioStreamBasicDescription &desc);
    void setFastMode(bool fast) { m_encoder->SetFastMode(fast); }
    bool encodeChunk(UInt32 npackets);
    void getMagicCookie(std::vector<uint8_t> *cookie);
    void setSource(const x::shared_ptr<ISource> &source) { m_src = source; }
    void setSink(const x::shared_ptr<ISink> &sink) { m_sink = sink; }
    ISource *src() { return m_src.get(); }
    const AudioStreamBasicDescription &getInputDescription() const
    {
	return m_input_desc.asbd;
    }
    const AudioStreamBasicDescription &getOutputDescription() const
    {
	return m_output_desc.asbd;
    }
    uint64_t samplesRead() const { return m_stat.samplesRead(); }
    uint64_t samplesWritten() const { return m_stat.samplesWritten(); }
    uint64_t framesWritten() const { return m_stat.framesWritten(); }
    double currentBitrate() const { return m_stat.currentBitrate(); }
    double overallBitrate() const { return m_stat.overallBitrate(); }
};

#endif
