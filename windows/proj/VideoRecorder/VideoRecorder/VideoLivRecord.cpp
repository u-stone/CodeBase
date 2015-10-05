#include "StdAfx.h"
#include "VideoLivRecord.h"
#include "errlog.h"

CVideoLivRecord::CVideoLivRecord(void)
{
	av_register_all();
	m_pAVFormatContext = 0;
	m_pAudioStream = 0;
	m_pVideoStream = 0;
	m_pAudioCodec = 0;
	m_pVideoCodec = 0;
	m_pOpt = 0;
	m_pAudioFrame = 0;
	m_pAudioBkFrame = 0;
	m_pVideoFrame = 0;
	m_pVideoBkFrame = 0;
	m_pVideoSwsctx = 0;
	m_pVideoSwrctx = 0;
	m_pAudioSwrctx = 0;

	m_Width = 800;
	m_Height = 600;
	m_bHasAudio = FALSE;
	m_bHasVideo = FALSE;

	m_t = 0;
	m_tincr = 0;
	m_tincr2 = 0;

	m_videoNextpts = 0;
	m_videoduriation = 0;
	m_audioNextpts = 0;
	m_audioduriation = 1000;
	m_videoduriation = 1000;
	m_AudioSamplesCount = 0;

	m_bEncodeVideo = FALSE;
	m_bEncodeAudio = FALSE;
}


CVideoLivRecord::~CVideoLivRecord(void)
{
	av_write_trailer(m_pAVFormatContext);
	if (m_bHasVideo){
		avcodec_close(m_pVideoStream->codec);
		av_frame_free(&m_pVideoFrame);
		av_frame_free(&m_pVideoBkFrame);
		sws_freeContext(m_pVideoSwsctx);
		swr_free(&m_pVideoSwrctx);
	}
	if (m_bHasAudio){
		avcodec_close(m_pAudioStream->codec);
		av_frame_free(&m_pVideoFrame);
		av_frame_free(&m_pVideoBkFrame);
		swr_free(&m_pAudioSwrctx);
	}
	if (!(m_pAVFormatContext->oformat->flags & AVFMT_NOFILE))
		avio_close(m_pAVFormatContext->pb);
	avformat_free_context(m_pAVFormatContext);
}

void CVideoLivRecord::InitRecoder(LPCSTR lpFileName,LONG lWidth,LONG lHeight,INT iKeyFrameInterval,int iOnlyVideo)
{
	m_Width = lWidth;
	m_Height = lHeight;
	m_videoduriation = iKeyFrameInterval;
	m_audioduriation = iKeyFrameInterval;

	int ret = 0;
	char filename[MAX_PATH] = {0};
	memcpy(filename, lpFileName, strlen(lpFileName));
// 	strcat(filename, ".");
// 	strcat(filename, FILE_SUFFIX);

	avformat_alloc_output_context2(&m_pAVFormatContext, NULL, NULL, filename);
	if (!m_pAVFormatContext){
		log("[CVideoLivRecord::InitRecoder] -- avformat_alloc_output_context2() error");
		return ;
	}
	//video
	if (m_pAVFormatContext->oformat->video_codec != AV_CODEC_ID_NONE){
		add_stream(&m_pVideoStream, &m_pVideoCodec, m_pAVFormatContext->oformat->video_codec);
		m_bHasVideo = TRUE;
		m_bEncodeVideo = TRUE;
	}
	//audio
	if (iOnlyVideo == 0 && m_pAVFormatContext->oformat->audio_codec != AV_CODEC_ID_NONE){
		add_stream(&m_pAudioStream, &m_pAudioCodec, m_pAVFormatContext->oformat->audio_codec);
		m_bHasAudio = TRUE;
		m_bEncodeAudio = TRUE;
	}
	if (m_bHasVideo){
		open_video(m_pVideoStream, m_pVideoCodec, m_pOpt);
	}
	if (m_bHasAudio){
		open_audio(m_pAudioStream, m_pAudioCodec, m_pOpt);
	}

	if (!(m_pAVFormatContext->oformat->flags & AVFMT_NOFILE)){
		ret = avio_open(&m_pAVFormatContext->pb, filename, AVIO_FLAG_WRITE);
		if (ret < 0){
			log("[CVideoLivRecord::InitRecoder] -- avio_open() error");
			return ;
		}
	}

	ret = avformat_write_header(m_pAVFormatContext, &m_pOpt);
	if (ret < 0){
		log("[CVideoLivRecord::InitRecoder] -- avformat_write_header() error");
		return ;
	}
}

void CVideoLivRecord::write_video_data(void* pBuffer, LONG len)
{
	write_video_frame(m_pVideoStream, pBuffer, len);
}

void CVideoLivRecord::write_audio_data(char* pBuffer, LONG len)
{
	write_audio_frame(m_pAudioStream, pBuffer, len);
}

void CVideoLivRecord::add_stream(AVStream** stream, AVCodec** codec, enum AVCodecID codec_id)
{
	int i = 0;
	*codec = avcodec_find_encoder(codec_id);
	if (!(*codec)){
		log("[CVideoLivRecord::add_stream] -- avcodec_find_decoder() error");
		return ;
	}
	(*stream) = avformat_new_stream(m_pAVFormatContext, *codec);
	if (!stream){
		log("[CVideoLivRecord::add_stream] -- avformat_new_stream() error");		
		return ;
	}
	(*stream)->id = m_pAVFormatContext->nb_streams - 1;
	AVCodecContext* avcc = (*stream)->codec;
	switch ((*codec)->type)
	{
	case AVMEDIA_TYPE_VIDEO:
		avcc->codec_id = codec_id;
		avcc->bit_rate = 400000;
		avcc->width = m_Width;
		avcc->height = m_Height;
		(*stream)->time_base.num = 1;
		(*stream)->time_base.den = STREAM_FRAME_RATE;
		avcc->time_base = (*stream)->time_base;
		avcc->gop_size = 12;
		avcc->pix_fmt = STREAM_PIX_FMT;
		if (avcc->codec_id == AV_CODEC_ID_MPEG2VIDEO){
			avcc->max_b_frames = 2;
		}
		if (avcc->codec_id == AV_CODEC_ID_MPEG1VIDEO){
			avcc->mb_decision = 2;
		}
		break;
	case AVMEDIA_TYPE_AUDIO:
		avcc->sample_fmt = (*codec)->sample_fmts ? (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
		avcc->bit_rate = 64000;
		avcc->sample_rate = 44100;
		if ((*codec)->supported_samplerates){
			avcc->sample_rate = (*codec)->supported_samplerates[0];
			for (i = 0; (*codec)->supported_samplerates[i]; ++i){
				if ((*codec)->supported_samplerates[i] == 44100){
					avcc->sample_rate = 44100;
				}
			}
		}
		avcc->channels = av_get_channel_layout_nb_channels(avcc->channel_layout);
		avcc->channel_layout = AV_CH_LAYOUT_STEREO;
		if ((*codec)->channel_layouts){
			avcc->channel_layout = (*codec)->channel_layouts[0];
			for (i = 0; (*codec)->channel_layouts[0]; ++i){
				if ((*codec)->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
					avcc->channel_layout = AV_CH_LAYOUT_STEREO;
			}
		}
		avcc->channels = av_get_channel_layout_nb_channels(avcc->channel_layout);
		(*stream)->time_base.num = 1;
		(*stream)->time_base.den = avcc->sample_rate;
		break;
	default:
		break;
	}
	if (m_pAVFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
		avcc->flags |= CODEC_FLAG_GLOBAL_HEADER;
}


BOOL CVideoLivRecord::open_video(AVStream *st, AVCodec* codec, AVDictionary* opt_arg)
{
	AVCodecContext* avcc = st->codec;
	AVDictionary* opt = 0;
	av_dict_copy(&opt, opt_arg, 0);

	int ret = avcodec_open2(avcc, codec, &opt);
	av_dict_free(&opt);
	if (ret < 0){
		log("[CVideoLivRecord::open_video] -- avcodec_open2() error");
		string str = av_make_error_string(64, ret);
		log(str);
		return FALSE;
	}

	m_pVideoFrame = alloc_picture(avcc->pix_fmt, m_Width, m_Height);
	if (!m_pVideoFrame){
		log("[CVideoLivRecord::open_video] -- alloc_picture() error");
		return FALSE;
	}
	m_pVideoBkFrame = NULL;
	if (avcc->pix_fmt != AV_PIX_FMT_RGBA){
		m_pVideoBkFrame = alloc_picture(AV_PIX_FMT_RGBA, m_Width, m_Height);
		if (!m_pVideoBkFrame){
			log("[CVideoLivRecord::open_video] -- alloc_picture(AV_PIX_FMT) error");
			return FALSE;
		}
	}
	return TRUE;
}

AVFrame* CVideoLivRecord::alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
{
	AVFrame* picture = av_frame_alloc();
	if (!picture)
		return NULL;

	picture->format = pix_fmt;
	picture->width = width;
	picture->height = height;

	int ret = av_frame_get_buffer(picture, 32);
	if (ret < 0){
		log("[CVideoLivRecord::alloc_picture] -- av_frame_get_buffer() error");
		return NULL;
	}

	return picture;
}

BOOL CVideoLivRecord::open_audio(AVStream *st, AVCodec* codec, AVDictionary* opt)
{
	AVCodecContext * avcc = st->codec;
	int nb_samples;
	int ret;
	AVDictionary* opt_dst = NULL;//必须初始化为空。

	av_dict_copy(&opt_dst, opt, 0);
	ret = avcodec_open2(avcc, codec, &opt_dst);
	av_dict_free(&opt_dst);
	if (ret < 0){
		log("[CVideoLivRecord::open_audio] -- avcodec_open2() error");
		return FALSE;
	}
	m_t = 0;
	m_tincr = 2 * M_PI * 110.0 / avcc->sample_rate;
	m_tincr2 = 2 * M_PI * 110.0 / avcc->sample_rate / avcc->sample_rate;

	if (avcc->codec->capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE){
		nb_samples = 10000;
	} else {
		nb_samples = avcc->frame_size;
	}
	
	m_pAudioFrame = alloc_audio_frame(avcc->sample_fmt, avcc->channel_layout, avcc->sample_rate, nb_samples);
	m_pAudioBkFrame = alloc_audio_frame(AV_SAMPLE_FMT_S16, avcc->channel_layout, avcc->sample_rate, nb_samples);

	m_pAudioSwrctx = swr_alloc();
	if (!m_pAudioSwrctx){
		log("[CVideoLivRecord::open_audio] -- swr_alloc() error");
		return FALSE;
	}
	av_opt_set_int       (m_pAudioSwrctx, "in_channel_count", avcc->channels,    0);
	av_opt_set_int       (m_pAudioSwrctx, "in_sample_rate",   avcc->sample_rate, 0);
	av_opt_set_sample_fmt(m_pAudioSwrctx, "in_sample_fmt",    AV_SAMPLE_FMT_S16, 0);
	av_opt_set_int       (m_pAudioSwrctx, "out_channel_count", avcc->channels,    0);
	av_opt_set_int       (m_pAudioSwrctx, "out_sample_rate",  avcc->sample_rate, 0);
	av_opt_set_sample_fmt(m_pAudioSwrctx, "out_sample_fmt",   avcc->sample_fmt,  0);

	if (swr_init(m_pAudioSwrctx) < 0){
		log("[CVideoLivRecord::open_audio] -- swr_init() error");
		return FALSE;
	}
	return TRUE;
}

BOOL CVideoLivRecord::write_audio_frame(AVStream *st, void* pBuffer, LONG len)
{
	AVCodecContext* avcc = st->codec;
	AVPacket pkt = {0};
	av_init_packet(&pkt);
	AVFrame* frame = get_audio_frame(st, pBuffer, len);
	int ret = 0;
	int dst_nb_samples = 0;
	if (frame){
		dst_nb_samples = (int)av_rescale_rnd(swr_get_delay(m_pAudioSwrctx, avcc->sample_rate) + frame->nb_samples,
			avcc->sample_rate, avcc->sample_rate, AV_ROUND_UP);
		av_assert0(dst_nb_samples == frame->nb_samples);
		ret = av_frame_make_writable(m_pAudioFrame);
		if (ret < 0){
			log("[CVideoLivRecord::write_audio_frame] -- av_frame_make_writable() error");
			return FALSE;
		}
		ret = swr_convert(m_pAudioSwrctx, m_pAudioFrame->data, dst_nb_samples, 
			             (const uint8_t**)frame->data, frame->nb_samples);
		if (ret < 0){
			log("[CVideoLivRecord::write_audio_frame] -- av_frame_make_writable() error");
			return FALSE;
		}
		frame = m_pAudioFrame;
		AVRational tmp = {1, avcc->sample_rate};

		frame->pts = av_rescale_q(m_AudioSamplesCount, tmp, avcc->time_base);
		m_AudioSamplesCount += dst_nb_samples;
	}
	int got_packet = 0;
	ret = avcodec_encode_audio2(avcc, &pkt, frame, &got_packet);
	if (ret < 0){
		log("[CVideoLivRecord::write_audio_frame] -- avcodec_encode_audio2() error");
		return FALSE;
	}
	if(got_packet){
		av_packet_rescale_ts(&pkt, avcc->time_base, st->time_base);
		pkt.stream_index = st->index;
		ret = av_interleaved_write_frame(m_pAVFormatContext, &pkt);
		//ret = write_audio_frame(m_pAudioStream, pBuffer, len);
		if (ret < 0){
			log("[CVideoLivRecord::write_audio_frame] -- write_audio_frame() error");
			return FALSE;
		}
	}
	return (frame || got_packet)? FALSE : TRUE;
}

static int count = 0;

BOOL CVideoLivRecord::write_video_frame(AVStream *st, void* pBuffer, LONG len)
{
	int got_packet = 0;
	AVFrame* frame = get_video_frame(st, pBuffer, len);
	if (!(m_pAVFormatContext->oformat->flags & AVFMT_RAWPICTURE)){
		AVPacket pkt = {0};
		av_init_packet(&pkt);
		AVCodecContext *avc = st->codec;

		int ret = avcodec_encode_video2(avc, &pkt, frame, &got_packet);
		if (ret < 0){
			log("[CVideoLivRecord::write_video_frame] -- avcodec_encode_video2() error");
			return FALSE;
		}
		if (got_packet){
			count++;
			/* rescale output packet timestamp values from codec to stream timebase */
			av_packet_rescale_ts(&pkt, st->codec->time_base, st->time_base);//这两个时间是不一样的，这里花了一下午时间找出问题来，汗!!!
			pkt.stream_index = st->index;
			ret = av_interleaved_write_frame(m_pAVFormatContext, &pkt);
		}
		if (ret < 0){
			log("[CVideoLivRecord::write_video_frame] -- error");
			return FALSE;
		}
	}
	return (frame || got_packet)? FALSE : TRUE;
}

AVFrame* CVideoLivRecord::get_video_frame(AVStream* st, void* pBuffer, LONG len)
{
	AVCodecContext *avc = st->codec;
	AVRational tmp = {1,1};
	//if (av_compare_ts(m_videoNextpts, st->codec->time_base, m_videoduriation, tmp) >= 0)
	//	return NULL;
	if (avc->pix_fmt != AV_PIX_FMT_RGBA){
		if (!m_pVideoSwsctx){
			m_pVideoSwsctx = sws_getContext(avc->width, avc->height, 
				AV_PIX_FMT_RGBA, avc->width, avc->height,avc->pix_fmt, 
				SCALE_FLAGS, NULL, NULL, NULL);
			if (!m_pVideoSwsctx){
				log("[CVideoLivRecord::get_video_frame] -- sws_getContext() error");
				return NULL;
			}
		}
		fill_rgba_picture(m_pVideoBkFrame, avc->width, avc->height, pBuffer, len);
		sws_scale(m_pVideoSwsctx, (const uint8_t* const*)m_pVideoBkFrame->data, m_pVideoBkFrame->linesize, 0,
			avc->height, m_pVideoFrame->data, m_pVideoFrame->linesize);
	} else {
		fill_rgba_picture(m_pVideoFrame, avc->width, avc->height, pBuffer, len);
	}
	m_pVideoFrame->pts = m_videoNextpts++;
	return m_pVideoFrame;
}

AVFrame* CVideoLivRecord::get_audio_frame(AVStream* st, void* pBuffer, LONG len)
{
	AVRational tmp = {1,1};

	if (av_compare_ts(m_audioNextpts, st->codec->time_base, 10, tmp) >= 0){
		return NULL;
	}
	AVFrame* frame = m_pAudioBkFrame;
	int16_t* q = (int16_t*)frame->data[0];
	//generate data
// 	int v = 0;
// 	for (int i = 0; i < frame->nb_samples; ++i){
// 		v = (int)(sin(m_t) * cos(m_t)* 10000);
// 		for (int j = 0; j < st->codec->channels; ++j){
// 			*q++ = v;
// 		}
// 		m_t += m_tincr;
// 		m_tincr += m_tincr2;
// 	}
	//write data
	memcpy(q, pBuffer, len);

	frame->pts = m_audioNextpts;
	m_audioNextpts += frame->nb_samples;
	return frame;
}


BOOL CVideoLivRecord::fill_rgba_picture(AVFrame* frame, int width, int height, void* pBuffer, int len)
{
	int ret = av_frame_make_writable(frame);
	if (ret < 0){
		log("[CVideoLivRecord::fill_picture] -- av_frame_make_writable() error");
		return FALSE;
	}

	memcpy(frame->data[0], pBuffer, len);

	return TRUE;
}

AVFrame* CVideoLivRecord::alloc_audio_frame(enum AVSampleFormat sample_fmt, uint64_t channel_layout, int smaple_rate, int nb_samples)
{
	AVFrame* frame = av_frame_alloc();
	int ret = 0;
	if (!frame){
		log("[CVideoLivRecord::alloc_audio_frame] -- av_frame_alloc() error");
		return 0;
	}
	frame->format = sample_fmt;
	frame->channel_layout = channel_layout;
	frame->sample_rate = smaple_rate;
	frame->nb_samples = nb_samples;

	if (nb_samples){
		ret = av_frame_get_buffer(frame, 0);
		if (ret < 0){
			log("[CVideoLivRecord::alloc_audio_frame] -- av_frame_get_buffer() error");
			return 0;
		}
	}
	return frame;
}

void CVideoLivRecord::write_arbitrary_av()
{
	av_dump_format(m_pAVFormatContext, 0, "E:\\test.mp4", 1);

	while (m_bEncodeAudio || m_bEncodeVideo){
		if (m_bEncodeVideo && (!m_bEncodeAudio || av_compare_ts(m_videoNextpts, m_pVideoStream->codec->time_base, 
			m_audioNextpts, m_pAudioStream->codec->time_base) <= 0)){
			m_bEncodeVideo = !write_video_frame(m_pVideoStream, 0, 0);
		} else {
			m_bEncodeAudio = !write_audio_frame(m_pAudioStream, 0, 0);
		}
	}
}


