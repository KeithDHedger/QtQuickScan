
This is intended to be a  simple GUI written in Qt6 for image scanners using libsane.
It will never be as crammed full of features as xsane or proprietry software,as the title says it's gonna be simple.

Just started so lots to do yet.

Usable,but at the moment you can only set resolution,select the device and set colour.
Images are hardcoded ( for now ) as pnm images sent to "/tmp/output.pnm"

To compile use:
./autogen --prefix=/usr
make

Optional:
sudo make install

The app  can be run directly from the app folder ( for now ) without installing.

DEPS:
libsane + libsane development files
Qt6 ( may run on Qt5 but you need to adjust the configure.ac file ).

TODO ( in no particular order ):
More options.
Previews.
Output filename.
Image conversions.
Prefs
Save current prefs
Progress bar.
Error messages (GUI)
About.
Help.

Etc. Etc.
