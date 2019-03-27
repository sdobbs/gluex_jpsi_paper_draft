# GlueX Paper Analysis Example

## Data

* Versions
** recon_2017-01_ver03, Software: sw_recon.xml
** part_2017-01_ver20, Software: sw_analysis.xml
* How to generate ROOT Trees:
** hd_root --config=loose.opts -PPLUGINS=jpsi_lp [REST_FILES]

## Simulations

* Versions
** Software: sw_sim.xml
* How to generate ROOT Trees:
** hd_root --config=loose.opts -PPLUGINS=jpsi_lp [REST_FILES]

## Analysis

* DSelector_jpsi.C
* DSelector_jpsi.h
** Run using runDSelector.C

* Scripts to perform fits and generate tables in paper
* Scripts to generate 
* Upper limit calculations