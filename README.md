mvwm
====

Modern Virtual Window Manager

[![Build Status](https://travis-ci.org/ThomasAdam/mvwm.svg?branch=master)](https://travis-ci.org/ThomasAdam/mvwm)

This is a clean-up effort of FVWM.  See the [TODO](https://github.com/ThomasAdam/mvwm/blob/master/TODO) file for more information about the aims.

For now, I've namespaced the entire code and files to live under a **mvwm** prefix.  At the moment, existing FVWM configuration files will work with MVWM, the ~/.fvwm directory will need renaming to ~/.mvwm, and if you're using ~/.fvwm2rc as opposed to ~/.fvwm/config, then ~/.fvwm2rc can be copied to ~/.mvwm2rc.  If you're copying configuration files, don't forget to rename Fvwm-specific things to their MVWM counterparts.

The source code is undergoing a security audit by me, this can be observed on the ta/audit-libs branch.

Any questions, email me.

-- Thomas Adam
