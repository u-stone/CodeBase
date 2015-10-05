#pragma once

#include "stdafx.h"
//#include "Mp4Buffer.h"

#define AV_PIX_FMT     AV_PIX_FMT_YUV420P
#define STREAM_FRAME_RATE  16
#define STREAM_PIX_FMT AV_PIX_FMT
#define SCALE_FLAGS SWS_BICUBIC
#define FILE_SUFFIX    "mp4"
class CVideoLivRecord
{
public:
	CVideoLivRecord(void);
	~CVideoLivRecord(void);

private:
	AVFormatContext *m_pAVFormatContext;
	AVStream        *m_pAudioStream;
	AVStream        *m_pVideoStream;
	AVCodec         *m_pAudioCodec;
	AVCodec         *m_pVideoCodec;
	AVDictionary    *m_pOpt;
	AVFrame         *m_pAudioFrame;
	AVFrame         *m_pAudioBkFrame;
	AVFrame         *m_pVideoFrame;
	AVFrame         *m_pVideoBkFrame;
	SwsContext      *m_pVideoSwsctx;
	//SwsContext      *m_pAudioSwsctx;
	SwrContext      *m_pVideoSwrctx;
	SwrContext      *m_pAudioSwrctx;

	int             m_Width, m_Height;//Video width & height
	BOOL            m_bHasAudio;
	BOOL            m_bHasVideo;

	double           m_t, m_tincr, m_tincr2;

	string          m_strFileName;

	int64_t         m_videoNextpts;
	int64_t         m_videoduriation;
	int64_t         m_audioNextpts;
	int64_t         m_audioduriation;	
	int64_t         m_AudioSamplesCount;

	BOOL            m_bEncodeVideo;
	BOOL            m_bEncodeAudio;

	
public:
	void InitRecoder(LPCSTR lpFileName,LONG lWidth,LONG lHeight,INT iKeyFrameInterval,int iOnlyVideo);
	void write_video_data(void* pBuffer, LONG len);
	void write_audio_data(char* pBuffer, LONG len);

	void write_arbitrary_av();

private:
	AVFrame* get_video_frame(AVStream* st, void* pBuffer, LONG len);
	AVFrame* get_audio_frame(AVStream* st, void* pBuffer, LONG len);
	BOOL write_audio_frame(AVStream *st, void* pBuffer, LONG len);
	BOOL write_video_frame(AVStream *st, void* pBuffer, LONG len);
	void add_stream(AVStream** stream, AVCodec** codec, enum AVCodecID codec_id);
	BOOL open_audio(AVStream *st, AVCodec* codec, AVDictionary* opt);
	BOOL open_video(AVStream *st, AVCodec* codec, AVDictionary* opt);
	AVFrame* alloc_picture(enum AVPixelFormat pix_fmt, int width, int height);
	AVFrame* alloc_audio_frame(enum AVSampleFormat sample_fmt, uint64_t channel_layout, int smaple_rate, int nb_samples);
	BOOL fill_rgba_picture(AVFrame* frame, int width, int height, void* pBuffer, int len);
};

