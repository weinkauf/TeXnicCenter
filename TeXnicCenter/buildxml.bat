@echo off
rem Copy all xml schema files to the output directories
rem $Id$
rem
@cd xmlschema
@for %%i in (*.xsd) do copy %%i ..\..\output\release
@for %%i in (*.xsd) do copy %%i ..\..\output\debug
@cd ..