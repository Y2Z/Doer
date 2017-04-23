# Doer

A notepad application with no user interface which sits in your tray.  
Comes in handy as a to-do list.

Can also help you keep those pretty Japanese smileys at hand.

![screenshot](https://cloud.githubusercontent.com/assets/1392048/24592087/2bcfb58e-17de-11e7-8c22-27a804c2f0f0.png)


## Controls

    Ctrl+Q:       Exit the program
    Ctrl+F / F11: Toggle fullscreen
    Esc:          Exit fullscreen / hide to tray


## Build

    qmake -qt5
    make clean && make -j 4


## Install

    sudo make install


## Uninstall

    sudo make uninstall
