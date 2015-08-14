#ifndef INPUTPROCESSOR_HPP
#define INPUTPROCESSOR_HPP

#include <QAudioOutput>
#include <QAudioInput>


class InputProcessor
{
public:
    InputProcessor();
    int getPitch() const;
private:

    QAudioDeviceInfo m_Inputdevice;
    QAudioDeviceInfo m_Outputdevice;
    QAudioFormat m_format;
    QAudioInput *m_audioInput;
    QAudioOutput *m_audioOutput;
    QIODevice *m_input;
    QIODevice *m_output;
    QByteArray m_buffer;
    int m_iVolume;

    void initializeAudio();
};

#endif // INPUTPROCESSOR_HPP
