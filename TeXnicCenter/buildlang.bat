@echo off
rem Copy all spelling files to the appropriate output directories
rem $Id$
rem

if not exist ..\output\release\language (
md ..\output\release\language
)

if not exist ..\output\debug\language (
md ..\output\debug\language
)

@cd ..\myspell
@for %%i in (*.aff;*.dic) do copy %%i ..\output\release\language
@for %%i in (*.aff;*.dic) do copy %%i ..\output\debug\language
@cd ..\TeXnicCenter

@for %%i in (GPL_english.htm GPL_german.htm Tips_english.txt Tips_german.txt UserImages.bmp Packages.xml) do copy %%i ..\output\release
@for %%i in (GPL_english.htm GPL_german.htm Tips_english.txt Tips_german.txt UserImages.bmp Packages.xml) do copy %%i ..\output\debug

