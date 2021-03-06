#ifndef CLIP_H
#define CLIP_H

#include <QWaitCondition>
#include <QMutex>
#include <QVector>

class Cacher;
class Effect;
class Transition;
class QOpenGLFramebufferObject;
struct Sequence;
struct Media;
struct MediaStream;

struct AVFormatContext;
struct AVStream;
struct AVCodec;
struct AVCodecContext;
struct AVFrame;
struct AVPacket;
struct SwsContext;
struct SwrContext;
class QOpenGLTexture;

struct ClipCache {
	AVFrame** frames;
	long offset;
	bool written;
	bool unread;
	int write_count;
	QMutex mutex;
};

/*struct ClipPlayback {

};*/

struct Clip
{
    Clip(Sequence* s);
	~Clip();
    Clip* copy(Sequence* s);
    void reset_audio();
	void reset();
	void refresh();

	// timeline variables
    Sequence* sequence;
    bool enabled;
    long clip_in;
    long timeline_in;
    long timeline_out;
    int track;
    bool undeletable;
    int load_id;
	QString name;
    long get_timeline_in_with_transition();
    long get_timeline_out_with_transition();
    quint8 color_r;
    quint8 color_g;
    quint8 color_b;
    long getLength();
    long getMediaLength(double framerate);
    void* media; // attached media
    int media_type;
    int media_stream;
	int getWidth();
	int getHeight();

	// other variables (should be "duplicated" in copy())
    QList<Effect*> effects;
    QVector<int> linked;
    Transition* opening_transition;
    Transition* closing_transition;

    // media handling
    AVFormatContext* formatCtx;
    AVStream* stream;
    AVCodec* codec;
    AVCodecContext* codecCtx;
    AVPacket* pkt;
	AVFrame* frame;

    bool pkt_written;
    bool reached_end;
    bool open;
    bool finished_opening;
	bool replaced;

    // caching functions
    bool multithreaded;
    Cacher* cacher;
    QWaitCondition can_cache;
	int cache_size;
    ClipCache cache_A;
    ClipCache cache_B;
    QMutex lock;
    QMutex open_lock;

    // video playback variables
	SwsContext* sws_ctx;
	QOpenGLFramebufferObject** fbo;
    QOpenGLTexture* texture;
    long texture_frame;

    // audio playback variables
	SwrContext* swr_ctx;
    int frame_sample_index;
    int audio_buffer_write;
    bool audio_reset;
    bool audio_just_reset;
    long audio_target_frame;
};

#endif // CLIP_H
