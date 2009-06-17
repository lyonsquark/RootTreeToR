#summary Description and Installation Instructions
#labels Featured

RootTreeToR v4.1
June 2009
Adam Lyon (lyon at fnal.gov)

See DESCRIPTION for Copyright
See LICENSE for license
See NEWS for latest changes

=Features=

RootTreeToR is an R package that allows reading data from Root into
R. It has these features:

  * Efficiently import data to an R data frame from Root Trees and
   Chains. Any Root expression valid for the TTree::Draw command may
   be used to bring data into R. The Root Tree may contain primitive
   types or complicated objects (though the later may require extra
   libraries loaded into Root - loading them is possible to do from R
   with this package). For data manipulation that is beyond
   TTree::Draw, you can write a fairly simple C++ helper class to
   manipulate the data and import to R. Sample classes are provided.

   You can also use Root Event Lists (created and manipulated from
   within R or Root) to restrict the data to import.

  * Export an R data frame to a Root Tree file as a Root Tree of simple
    types.

  * Import data from Root 1D Histograms (TH1F). 2D histograms will be
    possible in the future.

=Limitations=

 I have not attempted to build this package on Windows. The automatic
 build system for this package will surely not work on Windows.

 I have tested this package with Root v4 and it seems to work
 fine. When the package is loaded using Root v5, some warning messages
 appear about unresolved symbols. These messages seem to be coming
 from Root and not R. The package loads fine and I haven't seen any
 problems. But you may be wary.

=Disclaimer=

Although the author is unaware of mistakes in importing Root
information into R, YOU are responsible for checking your results with
Root. There will be minor differences due to round off errors. There
may be major differences due to bugs. You should repeat parts of your
analysis in Root and compare to R. If you find problems, please inform
the author, who assumes absolutely no responsibility or liability as
to the correctness of your results when you use this package.

=Pre-installation=

Read this section to find out what you need to do before you install
RootTreeToR.

==Setting up Root==

This package will be built against Root libraries. Therefore, Root
must be properly configured and your PATH and LD_LIBRARY_PATH (or
DYLD_LIBRARY_PATH for the Mac) environment variables must be set
correctly.  Furthermore, ROOTSYS must be set correctly. Be sure you do
all of these things *before* you start R and try building this
package.

==Setting up your R library area==

 On linux, you may not have permission to add R packages to the
default R library area. It is easy to create your own R library area
for your personal packages. Simply make a directory and tell R about
it by setting the R_LIBS environment variable to that directory
name. Even better, make a file in your home area called .Renviron
containing the one line...

R_LIBS=/path/to/your/R/library/area

If R cannot write to the default package area, it will automatically
write to the directory pointed to by R_LIBS.

==Versions of Root==

Since RootTreeToR is built against Root, the package is then tied to a
particular version of Root. It is possible to create multiple
instances of RootTreeToR if you use more than one version of Root (see
installation section below).

** It is important to note that if you upgrade to a new version of
Root, you will very likely need to reinstall this package so that it
is built against the new Root libraries. Using this package built
against particular Root libraries, but running it with a different
version of Root set up will probably lead to strange problems - at
best R will crash; at worst you may get results that are incorrect in
subtle ways that are difficult to catch.

WARNING: I have noticed that if R itself is built while
LD_LIBRARY_PATH points to Root libraries, then R will be locked in to
that particular version of Root. You can build RootTreeToR against a
different version of Root, but R will continue to use the libraries
that were around when it (R) was built. See the paragraph above about
the bad things that can happen if RootTreeToR is not used with the
version of Root that it was built with. I do not understand why R gets
locked in -- the work around is to build (or rebuild) R from the
sources and ensure that Root is *not* in the PATH or LD_LIBRARY_PATH
environment variables.

=Installation=

Below are the actual installation instructions for releases of RootTreeToR.

==SIMPLE INSTALLATION on LINUX (one version of Root)==

1) Download the RootTreeToR.tgz package and store in some
directory. (From the command line, you may be able to use wget or curl
to do this step easily).
2) Setup Root
3) Launch R
4) Issue this command:

install.packages("/path/to/directory/RootTreeToR.tgz", repos=NULL)

5) See getting started below

==COMPLEX INSTALLATION on LINUX (multiple versions of Root)==

If you need to be able to choose between multiple version of Root,
then follow these instructions. Above are instructions for making a
separate personal library area. If you haven't done that, do that now.
In your personal library area, make a new directory called
RootTreeToR-multi, and within that directory, make sub-directories
that indicate the version of Root you are using. Then for each version
of Root

1) Downlowd the RootTreeToR.tgz package and store in some directory
2) Setup the appropriate version of Root
3) Launch R
4) Issue this command

install.packages("/path/to/directory/RootTreeToR.tgz", repos=NULL,
                 lib="/path/to/my/special/lib/area")

For example,
install.packages("/home/lyon/RoootTreeToR.tgz", repos=NULL,
		 lib="/home/lyon/Rlibs/RootTreeToR-multi/v4_04_02")

5) See getting started below

----
==SIMPLE INSTALLATION on the MAC (one version of Root)==

For some reason, the install.packages command on the Mac does not
build the package. So do this instead OUTSIDE of R (do not launch R)...

1) Downlowd the RootTreeToR.tgz package and store in some directory
2) cd to the directory containing RootTreeToR.tgz
3) Unpack the archive with
tar xvzf RootTreeToR.tgz
4) Install the package with
R CMD INSTALL RootTreeToR/

5) See getting started below


=GETTING STARTED=

Note: On the Mac, Malloc error messages will appear. These do not
occur on Linux, so this seems to be a Mac only problem. The way around
these annoying messages is to, before running R, set the MallocLogFile
environment variable to "/dev/null". (e.g. export
MallocLogFile=/dev/null). This does *not* need to be done on Linux.

1) If not already in R, launch it.

2) Load the library. If you followed the simple instructions above, do
library(RootTreeToR)

if you followed the instructions for having multiple versions of
Root, do:
library(RootTreeToR, lib="/path/to/my/special/lib/area"), 
e.g. library(RootTreeToR, lib="/home/lyon/Rlibs/RootTreeToR-multi/v4_04_02")

3) You should see the Root welcome box appear. You may see unresolved
symbol warnings (especially with Root v5). If you do *not* see an
error message about RootTreeToR failing to load, then all is probably
ok. Continue onward.

4) For general help, issue the following command
?RootTreeToR
