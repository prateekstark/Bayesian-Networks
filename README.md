# Bayesian Networks

We are given the Bayesian Network, which can be represented in many formats. We will use the .bif format. BIF stands for Bayesian Interchange Format.

## Input Format

We will work with the alarm.bif network. Please have a look at this file to get a basic understanding of how this information relates to the Bayes net image above. A sample Bayes net is as follows:

variable “X” {

type discrete[2] { “True” “False” };

}

variable “Y” {

type discrete[2] { “True” “False” };

}

variable “Z” {

type discrete[2] { “True” “False” };

}

probability(“X”) { table 0.2 0.8 ; }

probability(“Y”) { table 0.4 0.6 ; }

probability(“Z” “X” “Y”) { table 0.2 0.4 0.3 0.5 0.8 0.6 0.7 0.5; }

This says that X, Y, and Z all have two values each. X and Y have no parents and prior P(X=True)=0.2, P(X=False)=0.8, and so on. Z has both X and Y as parents. Its probability table says P(Z=True|X=True, Y=True) = 0.2, P(Z=True|X=True, Y=False) = 0.4 and so on.

Our input network will have the Bayes net structure including variables and parents, but will not have probability values. We will use -1 to represent that the probability value is unknown.

Probability(“X”) { table -1 -1 ; } will represent that the prior probability of X is unknown and needs to be computed via learning. To learn these values we will provide a data file. Each line will be a patient record. All features will be listed in exactly the same order as in the .bif network and will be comma-separated. If a feature value is unknown we will use the special symbol “?” for it. There will be no more than 1 unknown value per row. 


## Output Format

The output will be the result of learning each probability value in the conditional probability tables. In other words, you need to replace all -1s with a probability value of up to four decimal places. Thus, your output is a complete alarm.bif network.


## Usage

```bash
./compile.sh #For compiling the project
./run.sh [fileName].bif [dataest].dat
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update the tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
