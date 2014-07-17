# for python 2.x
import sys
import os
import glob

os.system('cls')
print '---------------------------------------------------------'
print 'Clean and name uVision project or multi-project workspace'
print '---------------------------------------------------------'
print

multiExt = '.uvmpw'
projExt = '.uvproj'
usage = 'Usage: Copy to folder containing a *.uvproj (project) ' + \
        'or *.uvmpw (multi-project workspace) file and double-click ' + \
        'this file, or drag and drop a *.uvproj|*.uvmpw file onto this file.'

multiFile = ''
oldProjFiles = []
newProjFiles = []

def end():
    print
    raw_input('Press Enter to exit . . .')
    print

class FileInfo:
    # fullname: the absolute file name
    # dirname:  absolute directory of file
    # filename: file name without extension
    # basename: file name with extension
    # ext:      extension with dot
    fullname = ''
    dirname = ''
    basename = ''
    filename = ''
    ext = ''

    #ctor
    def __init__(self, name):
        self.fullname = os.path.abspath(name)
        self.dirname = os.path.dirname(self.fullname)
        self.basename = os.path.basename(self.fullname)
        self.filename, self.ext = os.path.splitext(self.basename)

    # Make absolute file path from relative path
    def makeAbsName(self, relativeFile):
        return self.dirname + os.path.sep + relativeFile

    # Remove files (use list of exceptions)
    def removeFiles(self, ext, exceptions=[]):
        infos = []
        for e in exceptions:
            infos.append(FileInfo(e))        
        print 'Removing *.' + ext + ' files',
        if len(exceptions) > 0:
            print 'except ',
        for i in range(len(infos)):
            if i > 0: print ', ',
            print infos[i].filename + '.' + ext,
        print '...'
        for f in glob.glob(self.dirname + os.path.sep + '*.' + ext):
            if f not in exceptions:            
                os.remove(f)

# use absolute file names        
def replace_text_in_file(oldFile, strFind, strReplace, newFile=None):
    renamed = True
    if newFile is None or newFile == '':
        newFile = oldFile + '.new'
        renamed = False
    with open(newFile, 'w') as fout:
            with open(oldFile, 'r') as fin:
                for line in fin:
                    fout.write(line.replace(strFind, strReplace))
    if os.path.exists(newFile):
        os.remove(oldFile)
        if not renamed:
            os.rename(newFile, oldFile)            
    else:
        print 'Failed to create new file'
    
    
def process_file(absFile):
    fi = FileInfo(absFile)
    #print fi.filename
    #print absFile +'\n'+ fi.fullname +'\n'+ fi.dirname +'\n'+ fi.basename +'\n'+ fi.filename +'\n'+ fi.ext
    #print absFile +'\n'+ dirname +'\n'+ basename +'\n'+ fileName +'\n'+ ext

    if fi.ext == multiExt:
        multiFile = absFile
        print
        print '-----------------------'
        print 'Multi Project Workspace'
        print '-----------------------'
        print fi.filename
        print
        new_name = raw_input('New name (blank for no change): ')
        if len(new_name) != 0:
            multiFile = fi.makeAbsName(new_name + fi.ext)
            os.rename(fi.fullname, multiFile)
        return multiFile
            
    elif fi.ext == projExt:
        print
        print '-------'
        print 'Project'
        print '-------'
        print fi.filename
        print         
        new_name = raw_input('New name (blank for no change): ')
        if len(new_name) != 0:
            #print fi.filename + ' || ' + new_name
            absFile = fi.makeAbsName(new_name + fi.ext)
            replace_text_in_file(fi.fullname, \
                 '<OutputName>'+fi.filename+'</OutputName>', \
                 '<OutputName>'+new_name+'</OutputName>', absFile)            
        return absFile
        
def remove_uvision_files(fi, projFiles):
    print
    print '----------------------------'
    print 'Removing extra uVision files'
    print '----------------------------'    
    fi.removeFiles('axf')
    fi.removeFiles('bak')
    fi.removeFiles('iex')
    fi.removeFiles('htm')
    fi.removeFiles('lnp')
    fi.removeFiles('map')
    fi.removeFiles('plg')
    fi.removeFiles('dep')
    fi.removeFiles('crf')
    fi.removeFiles('d')
    fi.removeFiles('o')
    fi.removeFiles('lst')
    fi.removeFiles('uvopt')
    fi.removeFiles('uvgui.*')
    
    fi.removeFiles('uvproj', projFiles)


# PROGRAM START
if len(sys.argv) == 1:

    # no file given, get contextual information about this file
    fi = FileInfo(__file__)

    #search for a multiproject file first
    files = glob.glob(fi.makeAbsName('*' + multiExt))

    if len(files) == 1:        
        multiFile = process_file(files[0])
        # search for project files
        files = glob.glob(fi.makeAbsName('*' + projExt))
        for f in files:
            oldProjFiles.append(f)
            newProjFiles.append(process_file(f))
        # update the multi-project file
        for i in range(len(oldProjFiles)):
            oldInfo = FileInfo(oldProjFiles[i])
            newInfo = FileInfo(newProjFiles[i])
            replace_text_in_file(multiFile, \
                '<PathAndName>.'+os.path.sep+oldInfo.filename+projExt+'</PathAndName>', \
                '<PathAndName>.'+os.path.sep+newInfo.filename+projExt+'</PathAndName>')
    elif len(files) > 1:
        print 'Error: more than 1 multi-project worspace ('+multiExt+') file found.'
        end()
        exit
    else:
        # no multi-project file
        files = glob.glob(fi.makeAbsName('*' + projExt))
        if len(files) == 1:
            oldProjFiles.append(files[0])
            newProjFiles.append(process_file(files[0]))
        elif len(files) > 1:
            print 'Error: more than 1 uVision project ('+projExt+') file found.'
            end()
            exit
        else:
            print 'No uVision project or multi-project files found.'
            print usage
    # remove files
    remove_uvision_files(fi, newProjFiles)
elif len(sys.argv) == 2:
    process_file(sys.argv[1])    
else:
    print usage    

end()
