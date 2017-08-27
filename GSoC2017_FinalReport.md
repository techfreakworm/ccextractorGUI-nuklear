# Finding work done in GSoC 2017:
##### (Or skip to [What's done in GSoC 2017](#what-is-done-in-gsoc))
Since, the project idea was to write GUI for [CCExtractor](www.ccextractor.org) and make the code fit in existing directory structure. </br>
So, I cloned the code from CCExtractor [Main repository](https://github.com/CCExtractor/ccextractor) to my [GSoC Project Repository](https://github.com/techfreakworm/ccextractorGUI-nuklear) and added GUI code to it. It would also make it easier to merge in CCExtractor Main repository, once the repository maintainers have made their final checks.

## Main GUI source files:
https://github.com/techfreakworm/ccextractorGUI-nuklear/src/GUI </br>

#### For compiling and testing:
Refer [README.md](https://github.com/techfreakworm/ccextractorGUI-nuklear/blob/master/README.md#compile-ccx-gui) in [GSoC Project Repository](https://github.com/techfreakworm/ccextractorGUI-nuklear).

# <a id="what-is-done-in-gsoc">What's done in GSoC</a>

*The Code is not merged, yet.*
1. Wrote GUI from scratch using [Nuklear](https://github.com/vurtun/nuklear) library in C language. GUI contains a tabbed view covering the most options CLI CCExtractor offers.
2. Code added to existing directory structure of [Main Repository](https://www.github.com/CCExtractor/ccextractor).
3. Modified autoconf compilation scripts for CCExtractor to build GUI along with CLI CCExtractor.
4. Added a `ccextractorGUI` project in existing`ccextractor.sln` Visual studio Solution, to compile CLI with GUI. All the dependencies required to compile are provided in the repository itself.
5. Wrote [documentation for GUI](https://github.com/techfreakworm/ccextractorGUI-nuklear/blob/master/guidoc.md). </br>
*Project is complete and usable (as proposed in GSoC proposal)*

# Work done before getting selected in Google Summer of Code 2017:
*Code is merged* (Since, changes committed were not squashed, providing link to every commit might be confusing)
1. Wrote Autoconf scripts for CLI CCExtractor compilation to remove hassle of writing and modifying Makefile(s) manually.
2. Wrote `.tar.gz`(tarball) package creation script.
3. Wrote `.rpm` package creation script.
4. Added OCR/Burned subs extraction support for Mac computers by writing autoconf scripts for Mac.

Regards,</br>
Mayank Gupta</br> 
techfreakworm@gmail.com
