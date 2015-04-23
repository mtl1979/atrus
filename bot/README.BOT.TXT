
BeShare Robot Skeleton Class v2.1.0
Originally by Alan Ellis (alan@bemail.org)
Additional commands added and maintained by Zaranthos
Updated to compile with MUSCLE 6.20 by Jeremy Friesner
Auto re-connect code added by Jeremy Friesner

This is a C++ class for Jeremy's bot sample, for those
of us who want a bot class instead of a C framework. =)


HOW TO BUILD
------------

- Download MUSCLE (6.20 or higher) and unpack it so that the 'muscle'
  directory is in the same directory as this one.
- type 'make' to build the example.


HOW TO MODIFY
-------------

Derive from BeShareBot::Bot

override the "Hook" functions and introduce your own functionality.

Read the other readme in this package for further info.

This code is public domain, do what you like with it (but please 
be considerate and don't run bots that are annoying the other BeShare 
users!  If you want to test your 'bot, it's better to do it on
your own MUSCLE server (i.e. have your bot connect to 'localhost' than
to do it on e.g. beshare.befaqs.com.  Thanks!)
