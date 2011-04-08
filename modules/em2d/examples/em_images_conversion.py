import IMP
import IMP.em2d as em2d
import os
"""
    Conversion of Electron Microscopy Images.

"""

# Read images
fn_selection=em2d.get_example_path("all-1z5s-projections.sel")

srw = em2d.SpiderImageReaderWriter()
trw = em2d.TIFFImageReaderWriter()
fn_images=em2d.read_selection_file(fn_selection)
images = em2d.read_images(fn_images,srw)
# write
fn_saved=em2d.create_filenames(3,"1z5s-projection","tif")
em2d.save_images(images,fn_saved,trw)
