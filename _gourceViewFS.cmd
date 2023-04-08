@echo off
set "repopath=%cd%"
cd "D:\Programme\Gource"


REM ~ gource.exe --load-config "%repopath%\configfs.txt" "%1"

REM ~ gource.exe --load-config "%repopath%\config.txt" --output-ppm-stream "%1.ppm" "%1"

REM ~ gource.exe --load-config "%repopath%\config.txt" --path "%1" -o - | ffmpeg -y -r 30 -f image2pipe -vcodec ppm -i - -vcodec libx264 "%1.mp4"
REM ~ --hide progress,dirnames,usernames,filenames,date,mouse
gource.exe --hide progress,dirnames,usernames,filenames,date,mouse --title "TeXnicCenter software development 2002 - 2013" --seconds-per-day 0.05 --file-idle-time 0 --multi-sampling -f -1920x1080  "%repopath%"

 REM ~ --padding 1 Camera view padding.
REM ~ --file-idle-time 0 not sure what it means to 'expire' files after 60 seconds of not changing. Will they disappear?
REM ~ --hash-seed for coloring
REM ~ --hide bloom,date,dirnames,files,filenames,mouse,progress,tree,users,usernames
pause
