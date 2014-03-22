# for python 2.x
import sys
import os
import glob

print 
print 'Clean and name uVision project'
print 

extension = '.uvproj'
usage = 'Usage: Copy to folder containing a '+extension+' file or ' + \
        'drag and drop a '+extension+' file onto this file.'
    
def remove_files(dirname, ext, exception=''):
    print 'Removing *.' + ext + ' files',
    if exception != '':
        print 'except "' + exception + '.' + ext + '"',
    print '...'    
    for f in glob.glob(dirname + os.path.sep + '*.' + ext):
        if f != dirname + os.path.sep + exception + '.' + ext:
            os.remove(f)

def replace(oldFile, newFile, strFind, strReplace):
    with open(newFile, 'w') as fout:
            with open(oldFile, 'r') as fin:
                for line in fin:
                    fout.write(line.replace(strFind, strReplace))
    if os.path.exists(newFile):
        os.remove(oldFile)
    else:
        print 'Failed to create new file'

def end():
    print
    raw_input('Press Enter to continue . . .')
    print

def process_file(absFile):
    dirname = os.path.dirname(absFile)
    basename = os.path.basename(absFile)
    fileName, ext = os.path.splitext(basename)
    #print absFile +'\n'+ dirname +'\n'+ basename +'\n'+ fileName +'\n'+ ext

    if ext != extension:
        print usage
        end()
        exit

    print 'Project: ' + fileName
    new_name = raw_input('New name (blank for no change)? ')    
    while len(new_name) > 0 and len(new_name) < 4:
        print 'New name must be at least 3 characters'
        new_name = raw_input('New name (blank for no change)? ')        
        print

    if len(new_name) != 0:        
        replace(absFile, dirname + os.path.sep + new_name + ext, fileName, new_name)
    else:
        new_name = fileName
        
    remove_files(dirname, 'axf')
    remove_files(dirname, 'bak')
    remove_files(dirname, 'iex')
    remove_files(dirname, 'htm')
    remove_files(dirname, 'lnp')
    remove_files(dirname, 'map')
    remove_files(dirname, 'plg')
    remove_files(dirname, 'dep')
    remove_files(dirname, 'crf')
    remove_files(dirname, 'd')
    remove_files(dirname, 'o')
    remove_files(dirname, 'lst')
    remove_files(dirname, 'uvopt')
    remove_files(dirname, 'uvgui.*')

    remove_files(dirname, 'uvproj', new_name)

if len(sys.argv) == 1:
    files = glob.glob(os.path.dirname(os.path.abspath(__file__)) + os.path.sep +'*'+extension)
    if len(files) == 1:
        process_file(files[0])
    elif len(files) > 1:
        print 'Error: more than 1 '+extension+' file found.'
        end()
        exit
    else:
        print 'No '+extension+' files found.'        
elif len(sys.argv) == 2:
    process_file(sys.argv[1])    
else:
    print usage    

end()
