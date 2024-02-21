# FOLDER_SYNCHRONIZATION

This work aims to synchronize between two folders (one defined as the source folder and the second as the destination folder). The content in the destination folder always depends on the content in the source folder.

There are a couple scenarios that should be taken in account: 

the creation and deletion of files/folders in the source folder;

the creation and deletion of files/folders in the destination folder;

files/folders being renamed on the source folder;

files/folders being renamed on the destination folder;

the changes in the content of files and folders present both on source and destination.
                                        
To perform this, a solo C++ program was developed which receives as an argument the path of the source folder, the path of the destination folder, the time set for each synchronization cycle, and the number of iterations in which it will be performed.

## Usage Example
` ./FOLDER_SYNCHRONIZATION "C:\Source_Folder" "C:\Destination_Folder" 10000 100 `
