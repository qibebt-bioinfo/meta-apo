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

Using an optimized machine-learning based algorithm and a small number of paired WGS-amplicon samples for training, Meta-Apo calibrates the gene distribution for large-scale 16S-amplicon samples, thus produces much more consistent diversity patterns between the two approaches. Using Meta-Apo, 16S-amplicon sequencing can accurately predict the gene profile of microbiomes, yet with only a small fraction of the cost of WGS sequencing. Moreover, since 16S-amplicon datasets outnumber WGS datasets by a factor of 10 to 100, Meta-Apo enables a much broader “functional” view of the Microbiome Data Universe than what is possible using WGS datasets.

# System Requirement and dependency

## Hardware Requirements

Meta-Apo only requires a standard computer with sufficient RAM to support the operations defined by a user. For typical users, this would be a computer with about 2 GB of RAM. For optimal performance, we recommend a computer with the following specs:

  RAM: 8+ GB  
  CPU: 4+ cores, 3.3+ GHz/core

## Software Requirements

OpenMP library is the C/C++ parallel computing library. Most Linux releases have OpenMP already been installed in the system. In Mac OS X, to install the compiler that supports OpenMP, we recommend using the Homebrew package manager:
```
brew install gcc
```

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
**a. Training for KO abundance calibration**
```
meta-apo-train -t wgs.KO.abd -T 16s.KO.abd -o meta-apo-model.txt
```
The output file “meta-apo-model.txt” is the mapping model. 

**b. Calibration for KO abundance**
```
meta-apo-calibrate -t 16s.all.ko.abd -m meta-apo-model.txt -o meta-apo-calibrated.abd
```
The source files for example of customized reference is available as [Supplementary](#supplementary).
# Example dataset
Here we provide a demo dataset (Dataset 1) with relative abundance of 622 WGS-microbiomes pairs in "examples" folder. In this package, "16s.KO.abd" is relative abundance of the amplicon microbiomes in 15 WGS-amplicon pairs for training, "wgs.KO.abd" is relative abundance of the WGS microbiomes in WGS-amplicon pairs for training, "16s.all.KO.abd" is relative abundance of all amplicon microbiomes for calibration.

To run the demo, you can either:
```
cd example
sh Readme
```
or type the following command:
```
meta-apo-train -t wgs.KO.abd -T 16s.KO.abd -o meta-apo-model.txt
meta-apo-calibrate -t 16s.all.ko.abd -m meta-apo-model.txt -o meta-apo-calibrated
```
Then the output file “meta-apo-model.txt” is the mapping model, and "meta-apo-calibrated" is the calibration rusult file.

This demo run should take less than 1 minute on a recommended computer.

# Tools in this package
**a. meta-apo-train**

It trains for KO abundance calibration. Run:
```
meta-apo-train -h
```
for detailed parameters.

**b. meta-apo-calibrate**

It calculates for KO abundance. Run:
```
meta-apo-calibrate -h
```
for detailed parameters.

# Supplementary

[Dataset 1](***) contains relative abundance of 622 WGS-amplicon pairs.
