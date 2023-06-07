from tkinter import filedialog, Label, Button, Entry, Tk, OptionMenu, StringVar
from sys import exit
from os import path, mkdir
from ctypes import CDLL

compressPath = "./c_files/compress.so"
compress = CDLL(compressPath)

if not path.exists("output"):
        mkdir("output")

#use global variables so that information can be remembered between runs of compression function
lastLocation = './'
outputDest = './output' 
filename = ''
filename2 = ''

# Functions for opening file explorer windows
def browseInput():
    global filename
    global lastLocation
    filename = filedialog.askopenfilename(initialdir = lastLocation + "/", #open at last place used by browser
                                          title = "Select a File",
                                          filetypes = ((".mp4", "*.mp4*"), ("all files", "*.*")))
    #Update last used location
    lastLocation = path.dirname(filename)
    # Change label contents
    label_inputPath1.configure(text="File opened: " + filename)

def browseInputTwo():
    global filename2
    global lastLocation
    filename2 = filedialog.askopenfilename(initialdir = lastLocation + "/", #open at last place used by browser
                                          title = "Select a File",
                                          filetypes = ((".mp4", "*.mp4*"), ("all files", "*.*")))
    #Update last used location
    lastLocation = path.dirname(filename2)
    # Change label contents
    label_inputPath2.configure(text="File opened: " + filename2)

def browseOutput():
    global outputDest
    global lastLocation
    outputDest = filedialog.askdirectory(initialdir = lastLocation, #open at last place used by browser
                                          title = "Select an output folder")
    #Update last used location
    lastLocation = path.dirname(outputDest)
    # Change label contents
    label_outputPath.configure(text="Output path: " + outputDest)

#Calls C function to compress
def compressButton():
     global filename
     print(f"input {filename} length {len(filename)}")
     global outputDest
     filenameClean = filename.replace(' ', '\ ').encode()
     outputDestClean = outputDest.replace(' ', '\ ').encode()
     outputName = entry_outputName.get().replace(' ', '\ ').encode()
     outputSize = entry_outputSize.get()
     preset = presetVar.get().encode()

     if filename == '':
        print("ERROR - NO INPUT FILE.")
        return
    
     if not outputSize == '' and float(outputSize) > 0:
        compress.twoPass(filenameClean, outputDestClean, outputName, outputSize, preset)
     else: compress.simple(filenameClean, outputDestClean, outputName, preset)

#Stitch 2 video files together
#TODO - DOES NOT WORK
def concatenate(filename, filename2, outputName, outputDest):
    '''
    #run(["ffmpeg", "-i", f"{filename}", "-c", "copy", "intermediate1.ts"], shell=True) #make temp file 1
    #run(["ffmpeg", "-i", f"{filename2}", "-c", "copy", "intermediate2.ts"], shell=True) #make temp file 2
    #run(["ffmpeg", "-i", "\"concat:intermediate1.ts|intermediate2.ts\"", "-c", "copy", f"{outputDest}/{outputName}.mp4"], shell=True) #runs concat command
    run(["mkfifo temp1 temp2"], shell=True)
    run([f"ffmpeg -y -i {filename} -c copy -bsf:v h264_mp4toannexb -f mpegts temp1 2> /dev/null & \ "], shell=True)
    run([f"ffmpeg -y -i {filename2} -c copy -bsf:v h264_mp4toannexb -f mpegts temp2 2> /dev/null & \ "], shell=True)
    run([f"ffmpeg -f mpegts -i \"concat:temp1|temp2\" -c copy -bsf:a aac_adtstoasc {outputDest}/{outputName}.mp4"], shell=True)
    '''

#TK WINDOW ELEMENT DECLARATIONS
#region
# Create the root window
window = Tk()
# Set window title
window.title('pyCompress!')
# Set window size
window.geometry("700x700")
#Set window background color
window.config(background = "white")

#Title label
label_title = Label(window,
                    text = "Video compressor using ffmpeg and Tkinter",
                    width = 100, height = 4,
                    fg = "blue")
#Input path label 1
label_inputPath1 = Label(window,
                        text = "File opened: -",
                        width = 100, height = 1,
                        fg = "blue")
#Input path label 1
label_inputPath2 = Label(window,
                        text = "File opened: -",
                        width = 100, height = 1,
                        fg = "blue")
#Output path label
label_outputPath = Label(window,
                        text = "Output path: ./output",
                        width = 100, height = 1,
                        fg = "blue")
#Input browse button
button_inputExplore = Button(window,
                        text = "Browse for input",
                        command = browseInput)
#Second Input browse button
button_inputExplore2 = Button(window,
                        text = "Browse for second input",
                        command = browseInputTwo)
#Label for output name text entry
label_outputName = Label(window, 
                        text = "Output name:")
#Output name text entry
entry_outputName = Entry(window,
                        width = 32,
                        bg = "white",
                        fg = "black")
#Output path browse button
button_outputExplore = Button(window,
                        text = "Browse for output path",
                        command = browseOutput)
#Label for output size
label_outputSize = Label(window, 
                        text = "Desired output size (MB):")
#Desired output size text entry
entry_outputSize = Entry(window,
                        width = 8,
                        bg = "white",
                        fg = "black")
#Label for audio bitrate reduction entry field
label_audioBitrate = Label(window, 
                        text = "Desired audio bitrate (kbit/s):")
#Desired audio bitrate text entry, if decimal is given then audio is scaled down by that factor
entry_audioBitrate = Entry(window,
                        width = 8,
                        bg = "white",
                        fg = "black")
#Label for compression preset dropdown
label_presets = Label(window, 
                        text = "Compression preset:")
#Dropdown menu of compression presets
presetList = ['ultrafast', 'superfast', 'veryfast', 'faster', 'fast', 'medium', 'slow', 'slower', 'veryslow']
presetVar = StringVar()
presetVar.set(presetList[2])
options_presets = OptionMenu(window,
                            presetVar,
                            *presetList)

#BUTTONS
#Button to run compressing process
button_compress = Button(window,
                        text = "Compress Video",
                        command = compressButton)
#Button to run compressing process
button_concat = Button(window,
                        text = "Concatenate Video",
                        command = concatenate)
#Button to exit program
button_exit = Button(window,
                    text = "Exit",
                    command = exit)

# Grid method is chosen for placing the widgets at respective positions in a table like structure by specifying rows and columns
label_title.grid(column = 1, row = 1, pady = [0, 3])
label_inputPath1.grid(column = 1, row = 2, pady = [3, 3])
label_inputPath2.grid(column = 1, row = 3, pady = [3, 3])
label_outputPath.grid(column = 1, row = 4, pady = [3, 3])
button_inputExplore.grid(column = 1, row = 5, pady = [3, 3])
button_inputExplore2.grid(column = 1, row = 6, pady = [3, 3])
button_outputExplore.grid(column = 1, row = 7, pady = [3, 3])
label_outputName.grid(column = 1, row = 8, pady = [3, 1])
entry_outputName.grid(column = 1, row = 9, pady = [1, 3])
label_outputSize.grid(column = 1, row = 10, pady = [3, 1])
entry_outputSize.grid(column = 1, row = 11, pady = [1, 3])
label_audioBitrate.grid(column = 1, row = 12, pady = [3, 1])
entry_audioBitrate.grid(column = 1, row = 13, pady = [1, 3])
label_presets.grid(column = 1, row = 14, pady = [1, 3])
options_presets.grid(column = 1, row = 15, pady = [1, 3])
button_compress.grid(column = 1, row = 16, pady = [10, 3])
button_concat.grid(column = 1, row = 17, pady = [10, 3])
button_exit.grid(column = 1,row = 18, pady = [10, 3])
#endregion

# Let the window wait for any events
print("Opening GUI...")
window.mainloop()