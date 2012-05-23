#!/usr/bin/python
#
# Apologies for bundling all of jinja with this. I'll remove it once I work out
# dependencies in setuptools.

from distutils.core import setup

setup (name='Shine',
       version='0.3',
       author='James McGill',
       author_email='jmcgill@plexer.net',
       url='https://sites.google.com/site/shinearduino/',
       package_dir = {'': 'lib'},
       packages = ['shine', 'jinja2', 'jinja2._markupsafe'],
       package_data = {'': ['tmpl/*.tmpl', 'library/*.*']},
       requires=['Jinja2'],
       scripts = ['shine-compiler', 'shine-configure'])
