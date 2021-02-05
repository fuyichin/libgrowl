Readme file for libgrowl
=====
This libgrowl is implemented in C program, there is another libgrowl implemented with java.

How to compile
-----
This program can be compiled in most unix platform including macOS. Working with Growl for Windows 2.0.9, growl-for-linux.

macOS
-----
There are 2+1 ways to compile on macOS

1. using the common unix make command:
$ make 
_or_
$ make -f Makefile.unix

2. use xcodebuild
$ xcodebuild
==> build/Release/libgrowl.a

$ xcodebuild -alltargets
==> build/Release/libgrowl.a
==> build/Release/growlnotify

3. use Xcode editor
Open libgrowl.xcodeproj file.

How to test growlnotify program
-----
$ growlnotify
==> send 'Hello, libgrowl'

To debug:
$ growlnotify -v (or -vv for more information)

What is Growl?
=====
Growl is a Mac OS notification system, it was created before Mac OS has its own Notification Center. Growl is available for Windows system as well. There are other developers who created growl-for-linux.
Growl is an open source software, it was abandone around end of 2020, the software was removed from App Store.
The lastest version is 2.1.3, the source code can be downloaded from github.


Note:
Growl for Windows 2.0.9 binary can be downloaded from https://github.com/briandunnington/growl-for-windows/releases/tag/final

Development
=====
Xcode
-----
Project format: Xcode-3.2-compatile
Indent: tab

VSCode
-----

Code function comment
-----
Use Doxygen style comment.
Try avoid using // but /* */
