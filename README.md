# meta-apo

![Version](https://img.shields.io/badge/Version-1.0%20)
![Release date](https://img.shields.io/badge/Release%20date-Nov.%2008%2C%202019-brightgreen)



## Contents

- [Introduction](#introduction)
- [System Requirement and dependency](#system-requirement-and-dependency)
- [Installation guide](#installation-guide)
- [Usage](#usage)
- [Example dataset](#example-dataset)
- [Tools in this package](#tools-in-this-package)
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
Meta-Apo consists of two steps: training and calibration.
In the training step, Meta-Apo builds a model by a small number of amplicon-WGS pairs using machine learning.

**a. Training for KO abundance calibration**
```
meta-apo-train -t wgs.training.KO.abd -T 16s.training.KO.abd -o meta-apo-model.txt
```
The parameter 't' requires the abundance table of WGS microbiomes in the WGS-amplicon pairs and the parameter 'T' requires the abundance table of amplicon microbiomes in the WGS-amplicon pairs.
The output file “meta-apo-model.txt” is the mapping model. 

In the calibration step, considering WGS results as the “golden standard”, Meta-Apo calibrates the predicted functional profiles of amplicon samples using model built in the training step.

**b. Calibration for KO abundance**
```
meta-apo-calibrate -t 16s.test.KO.abd -m meta-apo-model.txt -o 16s-calibrated.abd
```
The parameter 't' requires the abundance table of amplicon pairs to be calibrated.
The output file “16s-calibrated.abd” is the calibrated abundance table.

The source files for example of customized reference is available as [Supplementary](#supplementary).
# Example dataset
Here we provide a demo dataset in "examples" folder. In this package, "16s.training.KO.abd" is relative abundance of the 15 amplicon microbiomes in WGS-amplicon pairs for training, "wgs.training.KO.abd" is relative abundance of the 15 WGS microbiomes in WGS-amplicon pairs for training, "16s.test.KO.abd" is relative abundance of amplicon microbiomes for calibration.

To run the demo, you can either:
```
cd example
sh Readme
```
or type the following command:
```
meta-apo-train -t wgs.training.KO.abd -T 16s.training.KO.abd -o meta-apo-model.txt
meta-apo-calibrate -t 16s.test.ko.abd -m meta-apo-model.txt -o 16s-calibrated.abd
```
Then the output file “16s-model.txt” is the mapping model, and "16s-calibrated.abd" is the calibrated relative abundance of the amplicon microbiomes.

This demo run should take less than 1 minute on a recommended computer.


# Supplementary

[Dataset 1](***) contains 622 sample pairs of WGS and V3-V5 region 16S rRNA amplicon samples.
[Dataset 2](***) contains 295 sample pairs of WGS and V1-V3 region 16S rRNA amplicon samples.
[Dataset 3](***) contains 2,354 WGS samples and 5,350 V3-V5 16S rRNA amplicon samples.
[Dataset 4](***) contains 2,045 WGS samples and 2,186 V1-V3 16S rRNA amplicon samples.
