import os
import ycm_core

def Settings(**kwargs):
    return { 'flags': [
        '-x', 'c++',
        '-std=c++17',
        '-Wall',
        '-Wextra'
        '-Werror',
        '-isystem', 'include',
        '-isystem', 'library/catch2/include',
    ] }
