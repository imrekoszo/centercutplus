#ifndef WINAMP_DSP_H
#define WINAMP_DSP_H

// standard headers
#include <stdint.h>

// library headers
#include <windows.h>


// DSP plugin interface

// notes:
// any window that remains in foreground should optimally pass unused
// keystrokes to the parent (winamp's) window, so that the user
// can still control it. As for storing configuration,
// Configuration data should be stored in <dll directory>\plugin.ini
// (look at the vis plugin for configuration code)

struct winampDSPModule
{
    char *description;          // description
    HWND hwndParent;            // parent window (filled in by calling app)
    HINSTANCE hDllInstance;     // instance handle to this DLL (filled in by calling app)

    // configuration dialog (if needed)
    void (*Config)(struct winampDSPModule *this_mod);

    // 0 on success, creates window, etc
    int (*Init)(struct winampDSPModule *this_mod);

    // modify waveform samples: returns number of samples to actually write
    // (typically numsamples, but no more than twice numsamples,
    // and no less than half numsamples)
    // numsamples should always be at least 128. should, but I'm not sure
    int (*ModifySamples)(struct winampDSPModule *thisModule, uint8_t *samples,
                         int sampleCount, int bitsPerSample, int chanCount,
                         int sampleRate);

    void (*Quit)(struct winampDSPModule *this_mod);    // called when unloading

    void *userData; // user data, optional
};

struct winampDSPHeader
{
    int version;                            // DSP_HDRVER
    char *description;                      // description of library
    winampDSPModule* (*getModule)(int);     // module retrieval function
};

// exported symbols
typedef winampDSPHeader* (*winampDSPGetHeaderType)();

// header version: 0x20 == 0.20 == winamp 2.0
#define DSP_HDRVER 0x20

#endif //WINAMP_DSP_H
