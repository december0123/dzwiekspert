#include "inputprocessor.hpp"
#include <QUrl>
const int BufferSize = 14096;
InputProcessor::InputProcessor()
    :   m_Inputdevice(QAudioDeviceInfo::defaultInputDevice())
    ,   m_Outputdevice(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_audioInput(0)
    ,   m_audioOutput(0)
    ,   m_input(0)
    ,   m_iVolume(0)
    ,   m_buffer(BufferSize, 0)
{


}

void InputProcessor::initializeAudio()
{

    m_format.setSampleSize(16); //set sample sze to 16 bit
    m_format.setSampleType(QAudioFormat::UnSignedInt ); //Sample type as usigned integer sample
    m_format.setByteOrder(QAudioFormat::LittleEndian); //Byte order
    m_format.setCodec("audio/pcm"); //set codec as simple audio/pcm

    QAudioDeviceInfo infoIn(QAudioDeviceInfo::defaultInputDevice());
    if (!infoIn.isFormatSupported(m_format))
    {
        //Default format not supported - trying to use nearest
        m_format = infoIn.nearestFormat(m_format);
    }

    QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());

    if (!infoOut.isFormatSupported(m_format))
    {
       //Default format not supported - trying to use nearest
        m_format = infoOut.nearestFormat(m_format);
    }
    if (m_input != 0) {
        disconnect(m_input, 0, this, 0);
        m_input = 0;
    }

    m_audioInput = new QAudioInput(m_Inputdevice, m_format, this);
    m_audioOutput = new QAudioOutput(m_Outputdevice, m_format, this);
}

void MainWindow::readMore()
{
    //Return if audio input is null
    if(!m_audioInput)
        return;

    //Check the number of samples in input buffer
    qint64 len = m_audioInput->bytesReady();

    //Limit sample size
    if(len > 4096)
        len = 4096;
    //Read sound samples from input device to buffer
    qint64 l = m_input->read(m_buffer.data(), len);
    if(l > 0)
    {
        //Assign sound samples to short array
        short* resultingData = (short*)m_buffer.data();


        short *outdata=resultingData;
        outdata[ 0 ] = resultingData [ 0 ];

         int iIndex;
         if(ui->chkRemoveNoise->checkState() == Qt::Checked)
         {
                //Remove noise using Low Pass filter algortm[Simple algorithm used to remove noise]
                for ( iIndex=1; iIndex < len; iIndex++ )
                {
                    outdata[ iIndex ] = 0.333 * resultingData[iIndex ] + ( 1.0 - 0.333 ) * outdata[ iIndex-1 ];
                }
          }

         for ( iIndex=0; iIndex < len; iIndex++ )
         {
             //Cange volume to each integer data in a sample
             outdata[ iIndex ] = ApplyVolumeToSample( outdata[ iIndex ]);
         }

         //write modified sond sample to outputdevice for playback audio
          m_output->write((char*)outdata, len);

    }

}

int InputProcessor::getPitch() const
{
    return 50;
}
