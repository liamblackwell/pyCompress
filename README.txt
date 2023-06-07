A python application for smallifying video files.
'Should' work with any video (tested with MP4, WebM, AVI)

Run pyCompress.exe. A command prompt window will pop up as well.

Select a video with the file explorer button (and then change filetype in bottom right if not looking for mp4s).
After that, select where the output video should go (defaults to output folder). 

Desired size can be changed, and if one is given the program will do a 2-pass compression to get to target output size.
If target size is not given a *very* simple 1-pass compression is performed. This is only of use for large, uncompressed video
and if used on already small video files may even enlarge them.

When it's done (cmd printout will tell you), the compressed video will appear in the "output" folder. It's name will be whatever
you entered for the output name, or the original + "_Compressed" if none was given.
The output folder will be generated if not already there / on first operation.

The cmd window should only ever prompt you for action if you do something wrong like give it a filename that's already present.
You can just interact with the prompts it gives you like a normal cmd window, because it is one.

Sometimes temporary files may be left over from the 2-pass operation. You can safely delete these.

For concatenating videos, select 2 input videos with each browse button and press concat button to use. Doesn't compress, only stitches them together.

--------

Changelog
v3:
All non-GUI functionality rewritten in C!
File browsers now always reopen at the last used location regardless of which button opened the browser
Now it only works(?) in linux haha

v2.1:
Fixed the "Conversion failed" and "Frames left in buffer" issues
Added a check for the requested output size being too small. Displays this minimum in a printout if triggered.
Output name no longer a compulsory field - if none is given it will be named as the input + "_Compressed"

v2:
Added size specification - program performs 2-pass encoding to reach a specified output size, if one is provided.
Added button to browse for folder which output will be placed in.
Added seperate headers for title, file selected, and output path.
Added compression preset selection, with slower presets yielding better encoding. Most impactful on single-pass encoding.