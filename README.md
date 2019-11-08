# Meta-Apo

![Version](https://img.shields.io/badge/Version-1.0-brightgreen)
![Release date](https://img.shields.io/badge/Release%20date-Nov.%2008%2C%202019-brightgreen)



## Contents

- [Introduction](#introduction)
- [System Requirement and dependency](#system-requirement-and-dependency)
- [Installation guide](#installation-guide)
- [Usage](#usage)
- [Example dataset](#example-dataset)
- [Supplementary](#supplementary)

# Introduction

Meta-Apo (Metagenomic Apochromat) calibrates the predicted gene profiles from 16S-amplicon sequences using an optimized machine-learning based algorithm and a small number of paired WGS-amplicon samples as model training, thus produces diversity patterns that are much more consistent between amplicon- and WGS-based strategies.

The Meta-Apo takes the functional gene profiles of small number (e.g. 15) of WGS-amplicon sample pairs as training, and outputs the calibrated functional profiles of large-scale (e.g. > 1,000) amplicon samples. Currently the Meta-Apo requires functional gene profiles to be annotated using KEGG Ontology.



# System Requirement and dependency

## Hardware Requirements

Meta-Apo only requires a standard computer with >1GB RAM to support the operations defined by a user.

## Software Requirements

Meta-Apo only requires a C++ compiler (e.g. g++) to build the source code.

# Installation guide

## Automatic Installation (recommended)

At present, Meta-Apo provides a fully automatic installer for easy installation.

**a. Download the package**
```
git clone https://github.com/qibebt-bioinfo/meta-apo.git	
```

**b. Install by installer**
```
cd meta-apo
source install.sh
```

The package should take less than 1 minute to install on a computer with the specifications recommended above.

The example dataset could be found at “examples” folder. Check the “examples/Readme” for details about the demo run.

## Manual Installation

If the automatic installer fails, Dynamic Meta-Storms can still be installed manually.

**a. Download the package**
```
git clone https://github.com/qibebt-bioinfo/meta-apo.git	
```

**b. Configure the environment variables (the default environment variable configuration file is “~/.bashrc”)**
```
export MetaApo=Path to MetaApo
export PATH="$PATH:$MetaApo/bin/"
source ~/.bashrc
```
**c. Compile the source code**
```
cd meta-apo
make
```
# Usage
The Meta-Apo consists of two steps: a. training and b. calibration. Currently the Meta-Apo requires all functional gene profiles to be annotated using KEGG Ontology. 


**I. Training for KO abundance calibration** 

In the training step, Meta-Apo builds a model by a small number (e.g 15) of paired amplicon-WGS samples using machine learning. Each sample should be sequenced by both shotgun WGS and amplicon (e.g. 16S rRNA), then the parse their functional profiles. For WGS, we recommend to use HuMANn 2 [ref], and for amplicons we recommend to use PICRUSt 2 [ref].  

For training, the Meta-Apo accepts gene profiles of training paired samples in two formats:  

```
meta-apo-train -T training.wgs.ko.abd -t training.16s.ko.abd -o meta-apo.model
```
in which parameter “-T” assigns the gene relative abundance table of training WGS samples and “-t” assigns that of training amplicons. Orders of paired sample should be exactly matched in the input WGS and amplicon tables.  


The format of a gene profile table of training WGS samples:  
```
Sample	K00001	K00002	K00003	K00004	K00005	K00010	K00006	K00011	K00007	K00012	K00008	K00013	K00009
Sample_1	4.72E-06	0	0.000248375	0	6.36E-06	4.16E-06	0	0	0	0.00187845	0.000431322	0.00129624	0
Sample_2	0.00046572	0	0.000713339	2.15E-06	1.78E-05	6.55E-06	0	0	0	0.00123159	0.000427736	0.00124583	7.24E-06
...

```


The training amplicon table is in the same format, and order of each sample is exactly matched with the training WGS table.  

```
meta-apo-train -L training.wgs.list -t training.16s.list -o meta-apo.model
```

in which parameter “-L” assigns the file list of training WGS samples and “-t” assigns that of training amplicons. Orders of paired sample should be exactly matched in the input WGS and amplicon lists. In the input list, each line contains the path of one single sample’s gene profile.    
The format of a gene profile list of training WGS samples: 
```
Sample_1	Sample_2.ko.out
Sample_2	Sample_2.ko.out
...
```
And the format of a single sample’s gene profile:
```
#Gene output
K01153  53.07
K00881  2.172
K07726  2.48
K16924  17.664
K02077  101.677
K03698  10.143
K07461  41.5
K00097  100.787
...
```

b. Sample lists  



In the calibration step, considering WGS results as the “golden standard”, Meta-Apo calibrates the predicted functional profiles of amplicon samples using model built in the training step.   

The training amplicon list is in the same format, and order of each sample is exactly matched with the training WGS list.  


Then the output file “meta-apo.model” is the generated training model for calibration in the next step.  


**II. Calibration for KO abundance**  
In the calibration step, Meta-Apo calibrates the predicted functional profiles of more amplicon samples using the model built in the training step. The predicted gene profiles of amplicons for calibration should be processed in the same way as the training amplicon samples (e.g. by PICRUSt 2).  

For calibration, the Meta-Apo also accepts gene profiles of amplicon samples in two formats as well as the training step.  

a. Abundance tables  

```
meta-apo-calibrate -t 16s.ko.abd -m meta-apo.model -o 16s.ko.calibrated.abd
```
The output file “16s.ko.calibrated.abd” is the calibrated gene abundance table.  
b. Sample lists  
```
meta-apo-train -l 16s.ko.list -m meta-apo.model -o 16s.ko.calibrated.out
```
The output folder “16s.ko.calibrated.out” contains the calibrated gene profiles of each single input sample, and the file list of calibrated samples is also output to “16s.ko.calibrated.out.list”.  

# Example dataset
Here we provide a demo dataset in "examples" folder. This dataset contains 15 WGS-amplicon training pairs, and 622 amplicon samples for calibration. All samples were produced by Human Microbiome Project stage I. Gene profiles of WGS samples were directly analyzed by HuMANn 2. Amplicon samples were sequenced by V3-V5 region 16S rRNA sequencing, and gene profiles were inferred by PICRUSt 2.   

In this dataset, " training.wgs.ko.abd" is the gene relative abundance table of 15 training WGS samples, and "training.16s.ko.abd" is that of 15 matched training amplicon samples. The gene profiles of samples for calibration is in “16s.ko.abd”, and their meta-data is in “meta.txt”.  


To run the demo, you can either run the script “Readme”:  

```
cd example
sh Readme
```
or type the following command:
```
meta-apo-train -T training.wgs.ko.abd -t training.16s.ko.abd -o meta-apo.model

meta-apo-calibrate -t 16s.ko.abd -m meta-apo.model -o 16s.ko.calibrated.abd

```
Then the output file "16s.ko.calibrated.abd " is the calibrated relative abundance of the amplicon microbiomes.  

This demo run should take less than 2 minutes on a recommended computer.  



# Supplementary
We also provide more datasets for test run, which is used in the manuscript “Meta-Apo improves accuracy of 16S-amplicon-based prediction of microbiome function”. All samples were produced by Human Microbiome Project stage I. Gene profiles of WGS samples were directly analyzed by HuMANn 2. and gene profiles of amplicons were inferred by PICRUSt 2.  
[Dataset 1](***) contains 622 paired samples of WGS and V3-V5 region 16S rRNA amplicon samples (also integrated in the source code package as example dataset for demo run).  
[Dataset 2](***) contains 295 paired samples of WGS and V1-V3 region 16S rRNA amplicon samples.   
[Dataset 3](***) contains unpaired 2,354 WGS samples and 5,350 V3-V5 16S rRNA amplicon samples.  
[Dataset 4](***) contains unpaired 2,045 WGS samples and 2,186 V1-V3 16S rRNA amplicon samples.  
