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

Meta-Apo (Metagenomic Apochromat) calibrates the predicted gene profiles from 16S-amplicon sequences using an optimized machine-learning based algorithm and a small number of paired WGS-amplicon samples as model training, thus produces diversity patterns that are much more consistent between amplicon- and WGS-based strategies[Fig.1].

The Meta-Apo takes the functional gene profiles of small number (e.g. 15) of WGS-amplicon sample pairs as training, and outputs the calibrated functional profiles of large-scale (e.g. > 1,000) amplicon samples. Currently the Meta-Apo requires functional gene profiles to be annotated using KEGG Ontology.

![Fig.1. Calibration of predicted functional profiles of microbiome amplicon samples by a small number of amplicon-WGS sample pairs for training. ](http://bioinfo.single-cell.cn/images/Fig.1.jpg)  
<div align=center>Fig.1. Calibration of predicted functional profiles of microbiome amplicon samples by a small number of amplicon-WGS sample pairs for training.</div>



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

If the automatic installer fails, Meta-Apo can still be installed manually.

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

In the training step, Meta-Apo builds a model by a small number (e.g 15) of paired amplicon-WGS samples using machine learning. Each sample should be sequenced by both shotgun WGS and amplicon (e.g. 16S rRNA), then parse their functional profiles. For WGS, we recommend to use HuMANn 2 [1], and for amplicons we recommend to use PICRUSt 2 [2].  

For training, the Meta-Apo accepts gene profiles of training paired samples in two formats:

**a. Abundance tables**

```
meta-apo-train -T training.wgs.ko.abd -t training.16s.ko.abd -o meta-apo.model
```
in which parameter “-T” assigns the gene relative abundance table of training WGS samples and “-t” assigns that of training amplicons. Orders of paired samples should be exactly consistent in the input WGS and amplicon tables.  


The format of a gene profile table of training WGS samples:  
```
Sample	K00001	K00002	K00003	K00004	K00005	K00010	K00006	K00011	K00007
Sample1	0.1	0	0.3	0.1	0.1	0.1	0.1	0	0.2
Sample2	0.3	0.1	0.1	0	0.1	0.2	0	0.1	0.1
Sample3	0	0.2	0.1	0.3	0	0	0.4	0	0

...
SampleN	0	0.1	0.2	0.4	0	0	0.3	0	0
```


The training amplicon table is in the same format, and order of each sample is exactly consistent with the training WGS table.  

**b. Sample lists** 

```
meta-apo-train -L training.wgs.list -t training.16s.list -o meta-apo.model
```

in which parameter “-L” assigns the file list of training WGS samples and “-t” assigns that of training amplicons. Orders of paired samples should be exactly consistent in the input WGS and amplicon lists. In the input list, each line contains the path of one single sample’s gene profile.    
The format of a gene profile list of training WGS samples: 
```
Sample1	Sample2.ko.out
Sample2	Sample2.ko.out
Sample3	Sample3.ko.out
...
SampleN	SampleN.ko.out
```
And the format of a single sample’s gene profile:
```
#KO Count
K00001	0.1
K00003	0.3
K00004	0.1
K00005	0.1
K00010	0.1
K00006	0.1
K00007	0.2
```

The training amplicon list is in the same format, and order of each sample is exactly matched with the training WGS list.  


Then the output file “meta-apo.model” is the generated training model for calibration in the next step.  


**II. Calibration for KO abundance**

In the calibration step, Meta-Apo calibrates the predicted functional profiles of more amplicon samples using the model built in the training step. The predicted gene profiles of amplicons for calibration should be processed in the same way as the training amplicon samples (e.g. by PICRUSt 2).  

For calibration, the Meta-Apo also accepts gene profiles of amplicon samples in two formats as well as the training step.  

**a. Abundance table**  

```
meta-apo-calibrate -t 16s.ko.abd -m meta-apo.model -o 16s.ko.calibrated.abd
```
The output file “16s.ko.calibrated.abd” is the calibrated gene abundance table. 

**b. Sample list**  
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

# Reference
1. Abubucker, S., et al., _Metabolic Reconstruction for Metagenomic Data and Its Application to the Human Microbiome_. 2012.
2. Langille, M.G., et al., _Predictive functional profiling of microbial communities using 16S rRNA marker gene sequences. Nat Biotechnol_, 2013. 31(9): p. 814-21.
