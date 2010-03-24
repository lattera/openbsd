#!/bin/sh
#
# $Id$

for n in ${a#*=}; do echo ${n}; done                                      
for n in "${a#*=}"; do echo ${n}; done                                      

for n in ${a##*=}; do echo ${n}; done                                      
for n in "${a##*=}"; do echo ${n}; done                                      

for n in ${a%=*}; do echo ${n}; done                                      
for n in "${a%=*}"; do echo ${n}; done                                      

for n in ${a%%=*}; do echo ${n}; done                                      
for n in "${a%%=*}"; do echo ${n}; done                                      
