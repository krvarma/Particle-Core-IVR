
// --------------------------------------------------------------------------------
// An Arduino library for communicating with the Emic 2 Text-to-Speech Module.
//
// MIT license.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//      **********************************************************************
//      Designed for the Emic 2 Text-to-Speech Module, developed by
//        Grand Idea Studio and Parallax:
//      http://www.grandideastudio.com/portfolio/emic-2-text-to-speech-module/
//
//      You can pick one up in the Adafruit Store:
//      http://www.adafruit.com/products/924
//      **********************************************************************
//
// --------------------------------------------------------------------------------
#include "Emic2TTS.h"


// Initialize the connection to the Emic2.
/*

Code taken from the Emic2 Demo sketch written by Joe Grand [www.grandideastudio.com]

*/

void Emic2TtsModule::init(Stream& stream) {
    pSerial = &stream;
    
    pSerial->write('\n');
    
    char ch = pSerial->read();
    
    while (ch != ':'){
        ch = pSerial->read();
        
        Serial.println(ch);
        delay(10);
    }
    
    pSerial->flush();
}

//! Play English language speaking demo.
void Emic2TtsModule::playSpeakingDemo() {
    sendCommand('D', 0);
}

//! Play English language singing demo.
void Emic2TtsModule::playSingingDemo() {
    sendCommand('D', 1);
}

//! Play Spanish language speaking demo.
void Emic2TtsModule::playSpanishDemo() {
    sendCommand('D', 2);
}

//! Set the volume of speech in decibels, from -48 to 18.
void Emic2TtsModule::setVolume(int volume) {
    sendCommand('V', volume);
}

//! Set the voice to speak in.
void Emic2TtsModule::setVoice(EmicVoice voice) {
    sendCommand('N', voice);
}

//! Set the rate of speech in words per minute, from 75 to 600.
void Emic2TtsModule::setWordsPerMinute(int rate) {
    sendCommand('W', rate);
}

//! Set the language to speak in.
void Emic2TtsModule::setLanguage(EmicLanguage language) {
    sendCommand('L', language);
}

//! Set the text to speech parser to use.
void Emic2TtsModule::setParser(EmicParser parser) {
    sendCommand('P', parser);
}

//! Reset the Emic 2 to the default values.
void Emic2TtsModule::restoreDefaults() {
    sendCommand('R');
}

//! Speak the passed in value, essentially a wrapper function for the function in Print.
void Emic2TtsModule::say(const String &string) {
    pSerial->write('S');
    
    char szString[64];
    
    string.toCharArray(szString, 64);
    
    pSerial->write(szString);
    sendTerminatorAndWait();
}

//! Speak the passed in value, essentially a wrapper function for the function in Print.
void Emic2TtsModule::say(const char s[]) {
    pSerial->write('S');
    pSerial->write(s);
    sendTerminatorAndWait();
}

//! Speak the passed in value, essentially a wrapper function for the function in Print.
void Emic2TtsModule::say(char c) {
    pSerial->write('S');
    pSerial->write(c);
    sendTerminatorAndWait();
}

//! Speak the passed in value, essentially a wrapper function for the function in Print.
void Emic2TtsModule::say(unsigned char c, int base) {
    pSerial->write('S');
    pSerial->write(c);
    sendTerminatorAndWait();
}

//! Speak the passed in value, essentially a wrapper function for the function in Print.
void Emic2TtsModule::say(int i, int base) {
    pSerial->write('S');
    pSerial->write(i);
    sendTerminatorAndWait();
}

//! Speak the passed in value, essentially a wrapper function for the function in Print.
void Emic2TtsModule::say(unsigned int i, int base) {
    pSerial->write('S');
    pSerial->write(i);
    sendTerminatorAndWait();
}

//! Speak the passed in value, essentially a wrapper function for the function in Print.
void Emic2TtsModule::say(long i, int base) {
    pSerial->write('S');
    pSerial->write(i);
    sendTerminatorAndWait();
}

//! Speak the passed in value, essentially a wrapper function for the function in Print.
void Emic2TtsModule::say(unsigned long i, int base) {
    pSerial->write('S');
    pSerial->write(i);
    sendTerminatorAndWait();
}

//! Speak the passed in value, essentially a wrapper function for the function in Print.
void Emic2TtsModule::say(double i, int digits) {
    pSerial->write('S');
    pSerial->write(i);
    sendTerminatorAndWait();
}

//! Send a command to the Emic 2 and wait for it to respond.
void Emic2TtsModule::sendCommand(char command) {
    pSerial->write(command);
    sendTerminatorAndWait();
}

//! Send a command and parameter to the Emic 2 and wait for it to respond.
void Emic2TtsModule::sendCommand(char command, int param) {
    pSerial->write(command);
    pSerial->write(param);
    sendTerminatorAndWait();
}

//! Terminate a command and wait for the Emic 2 to respond.
void Emic2TtsModule::sendTerminatorAndWait() {
    pSerial->write('\n');
    
    while (pSerial->read() != ':');
}