import os
import torch
import torch.nn as nn
import torch.nn.functional as functional
import numpy as np
from tabulate import tabulate
import matplotlib.pyplot as plt
import pickle


# class for ML model.
class MLModel(nn.Module):
    def __init__(self, layerCount, layerSize, activationFunction):
        super(MLModel, self).__init__()
        if layerCount == 2:
            self.layer1 = nn.Linear(784, 10)
            self.layerCount = 2
            self.activationFunction = activationFunction
        elif layerCount == 3:
            self.activationFunction = activationFunction
            self.layer1 = nn.Linear(784, layerSize)
            self.layer2 = nn.Linear(layerSize, 10)
            self.layerCount = 3
        elif layerCount == 4:
            self.activationFunction = activationFunction
            self.layer1 = nn.Linear(784, layerSize)
            self.layer2 = nn.Linear(layerSize, layerSize)
            self.layer3 = nn.Linear(layerSize, 10)
            self.layerCount = 4
# Forward function
    def forward(self, x):
        if self.layerCount == 2:
            output = self.layer1(x)
        elif self.layerCount == 3:
            hidden1 = self.layer1(x)
            activation1 = self.activationFunction(hidden1)
            output = self.layer2(activation1)
        elif self.layerCount == 4:
            hidden1 = self.layer1(x)
            activation1 = self.activationFunction(hidden1)
            hidden2 = self.layer2(activation1)
            activation2 = self.activationFunction(hidden2)
            output = self.layer3(activation2)
        last_touch = torch.softmax(output, dim=1)
        return last_touch

# Function for train
def trainer(model, optimizer, epoch_no):
    train_losses = []
    avg_train_accuracy = 0

    validation_losses = []
    avg_validation_accuracy = 0



    # For training 10 times
    for i in range(1, 11):
        train_loss_for_plot = []
        validation_loss_for_plot = []

        train_loss, validation_loss, train_loss_for_plot, validation_loss_for_plot = trainer_helper(model, optimizer,
                                                                                                    epoch_no,
                                                                                                    train_loss_for_plot,
                                                                                                    validation_loss_for_plot)

        train_losses.append(train_loss)
        validation_losses.append(validation_loss)

        well_trained = sum(torch.argmax(model(x_train), dim=1) == y_train)  # check if the labels equal to each other
        train_accuracy = well_trained / len(y_train) # divide well trained ones to length
        avg_train_accuracy += train_accuracy

        well_validated = sum(torch.argmax(model(x_validation), dim=1) == y_validation)
        validation_accuracy = well_validated / len(y_validation)
        avg_validation_accuracy += validation_accuracy

    avg_train_accuracy = avg_train_accuracy / 10  # calculate average
    avg_validation_accuracy = avg_validation_accuracy / 10

    with torch.no_grad():   # returns avg parameters and the model
        return [np.average(validation_losses), avg_validation_accuracy.item(), np.average(train_losses),
                avg_train_accuracy.item()], model

# Implemented last train as a different function in order to return min_validation_loss and min_training_loss. Also it does not loops for 10 times
def trainer_last(model, optimizer, epoch_no):
    train_losses = []
    avg_train_accuracy = 0

    validation_losses = []
    avg_validation_accuracy = 0

    min_training_loss = [float('inf')]
    min_validation_loss = [float('inf')]



    train_loss_for_plot = []
    validation_loss_for_plot = []

    train_loss, validation_loss, train_loss_for_plot, validation_loss_for_plot = trainer_helper(model, optimizer,
                                                                                                    epoch_no,
                                                                                                    train_loss_for_plot,
                                                                                                    validation_loss_for_plot)

    if np.average(min_validation_loss) > np.average(validation_loss_for_plot):  # check if the validation loss is greater than the temp value. If it is update min loss.
        min_validation_loss = validation_loss_for_plot
        min_training_loss = train_loss_for_plot

    train_losses.append(train_loss)
    validation_losses.append(validation_loss)

    well_trained = sum(torch.argmax(model(x_train), dim=1) == y_train)
    train_accuracy = well_trained / len(y_train)
    avg_train_accuracy += train_accuracy

    well_validated = sum(torch.argmax(model(x_validation), dim=1) == y_validation)
    validation_accuracy = well_validated / len(y_validation)
    avg_validation_accuracy += validation_accuracy

    avg_train_accuracy = avg_train_accuracy
    avg_validation_accuracy = avg_validation_accuracy

    with torch.no_grad():
        return [np.average(validation_losses), avg_validation_accuracy, np.average(train_losses),
                avg_train_accuracy], model, min_validation_loss, min_training_loss

# Helper function for train.
def trainer_helper(model, optimizer, epoch_no, train_loss_for_plot, validation_loss_for_plot):
    for epoch in range(epoch_no):
        model.train()

        optimizer.zero_grad()
        cross_entropy_loss = nn.CrossEntropyLoss()  # If I don't do this function does not work, found the usage from Internet forums

        x_train_model = model(x_train)
        train_loss = cross_entropy_loss(x_train_model, y_train)
        train_loss_for_plot.append(train_loss.item())

        train_loss.backward()  # backpropagate
        optimizer.step()

        x_validation_model = model(x_validation)
        validation_loss = cross_entropy_loss(x_validation_model, y_validation)
        validation_loss_for_plot.append(validation_loss.item())

    return train_loss, validation_loss, train_loss_for_plot, validation_loss_for_plot


# we load all the datasets of Part 3
x_train, y_train = pickle.load(open("data/mnist_train.data", "rb"))
x_validation, y_validation = pickle.load(open("data/mnist_validation.data", "rb"))
x_test, y_test = pickle.load(open("data/mnist_test.data", "rb"))

x_train = x_train / 255.0
x_train = x_train.astype(np.float32)

x_test = x_test / 255.0
x_test = x_test.astype(np.float32)

x_validation = x_validation / 255.0
x_validation = x_validation.astype(np.float32)

# and converting them into Pytorch tensors in order to be able to work with Pytorch
x_train = torch.from_numpy(x_train)
y_train = torch.from_numpy(y_train).to(torch.long)

x_validation = torch.from_numpy(x_validation)
y_validation = torch.from_numpy(y_validation).to(torch.long)

x_test = torch.from_numpy(x_test)
y_test = torch.from_numpy(y_test).to(torch.long)

# Initializes the table that visualizes the outcomes.
def table_init(layer_nos, epochs, layer_sizes, activation_funcs, learning_rates):
    data = []
    for layer_no in layer_nos:
        for epoch in epochs:
            if layer_no != 2:  # if hidden layers add new columns
                for layer_size in layer_sizes:
                    for activation_func in activation_funcs:
                        temp = [
                            " No: " + str(layer_no) + " | Epoch: " + str(epoch) + " | Size: " + str(layer_size) + " | Func: " + str(activation_func)]
                        data.append(temp)
            else:  # else print no hidden layer
                temp = [" No: " + str(layer_no) + " | Epoch: " + str(epoch) + " | No Hidden Layer " + "| No Hidden Layer"]
                data.append(temp)
    return data


def print_table(data, learning_rates):
    print(tabulate(data, headers=[' Layers | Epochs | LayerSizes | Activator '] + learning_rates, tablefmt="fancy_grid"))



# Python dictionaries are used because it is easier than defining a class. If the code were in cpp I would define classes for that purpose.
def init_dictionaries():
    learning_rate_dict = {
        0.1: [],
        0.001: [],
        0.0001: []
    }

    #
    relu = str(nn.ReLU())
    tanh = str(nn.Tanh())
    sigmoid = str(nn.Sigmoid())
    activation_function_dict = {
        relu: [],
        tanh: [],
        sigmoid: []
    }

    return learning_rate_dict, activation_function_dict


# Function to plot the loss figure
def plot_figure(training_loss, validation_loss):
    figure = plt.figure(figsize=(10, 8))
    plt.plot(training_loss, label='Training Loss')
    plt.plot(validation_loss, label='Validation Loss')
    plt.xlabel('Epoch No')
    plt.ylabel('Loss')
    plt.legend()
    plt.tight_layout()
    figure.savefig('last_loss_plot_png', bbox_inches='tight')


# Function to print best learning rate and activation function from the dictionaries
def print_best(learning_rate_dict, activation_function_dict):
    best_learning_rate = max(learning_rate_dict, key=learning_rate_dict.get)
    best_activation_func = max(activation_function_dict, key=activation_function_dict.get)
    print(learning_rate_dict)
    print("Best Learning Rate is: ", best_learning_rate)
    print(activation_function_dict)
    print("Best Activation Function is: ", best_activation_func)

# function for calculating confidence interval
def calculate_confidence_interval(avg_parameters):
    error = 1 - avg_parameters[1]
    confidence_interval = 100 * (1.96 * (error * (1 - error) / len(y_validation)) ** 0.5)
    return confidence_interval

# Helper function to save the table, print parameters, save the model and plot figure in the end
def finish(data, learning_rates, best_hyperparameter_loss, best_hyperparameters, device, learning_rate_dict, activation_function_dict):
    with open('validationAccuracy_table.txt', 'w') as filename:
        filename.write(tabulate(data, headers=learning_rates, tablefmt="fancy_grid"))

    print("Best loss: ", best_hyperparameter_loss, "/ hyper parameters : ", best_hyperparameters)

    ml_model = MLModel(best_hyperparameters[2], best_hyperparameters[3], best_hyperparameters[4])

    ml_model.to(device)
    optimizer = torch.optim.Adam(ml_model.parameters(), lr=best_hyperparameters[1])

    avg_parameters, ml_model, validation_loss, training_loss = trainer_last(ml_model, optimizer,
                                                                            best_hyperparameters[0])

    print("Validation Loss is: ", avg_parameters[0], " Validation Accuracy is: ", avg_parameters[1].item(), " Train Loss is: ",
          avg_parameters[2], " Train Accuracy is: ", avg_parameters[3].item())

    plot_figure(training_loss, validation_loss)

    total_correct_test = sum(torch.argmax(ml_model(x_test), dim=1) == y_test)
    test_accuracy = total_correct_test / len(y_test)

    cross_entropy_loss = nn.CrossEntropyLoss()
    test_ce_loss = cross_entropy_loss(ml_model(x_test), y_test)

    print("Test Loss is: ", test_ce_loss.item(), " Test Accuracy is: ", test_accuracy.item())

    torch.save(ml_model.state_dict(), 'model.pth')

    with open('hyper_parameters.txt', 'w') as filename:
        filename.write(str(best_hyperparameters))

    print_best(learning_rate_dict, activation_function_dict)

def main():
    use_cuda = 0
    device = torch.device('cuda' if use_cuda else 'cpu')

    activation_funcs = [nn.Sigmoid(), nn.Tanh()]
    layer_nos = [2, 4]
    layer_sizes = [8, 132]
    learning_rates = [0.1, 0.0001]
    epochs = [30, 80]

    best_hyperparameters = (float('-inf'), float('-inf'), float('-inf'), float('-inf'), None)
    best_hyperparameter_loss = float('inf')   # initialized as max value

    data = table_init(layer_nos, epochs, layer_sizes, activation_funcs, learning_rates)
    learning_rate_dict, activation_function_dict = init_dictionaries()

    # Loop in learning rates
    for learning_rate in learning_rates:

        row = 0

        # Loop in learning numbers
        for layer_no in layer_nos:

            print_table(data, learning_rates)

            # Loop in epochs
            for epoch in epochs:
                if layer_no > 2:   # if layer number is greater than 2 there are hidden layers to consider
                    for layer_size in layer_sizes:
                        for activation_func in activation_funcs:
                            ml_model = MLModel(layer_no, layer_size, activation_func)
                            ml_model.to(device)
                            optimizer = torch.optim.Adam(ml_model.parameters(), lr=learning_rate)
                            avg_parameters, ml_model = trainer(ml_model, optimizer, epoch)

                            confidence_interval = calculate_confidence_interval(avg_parameters)
                            data[row].append(str("%.2f" % (avg_parameters[1]*100)) + "% +- " + str("%.2f" % confidence_interval) + "%")

                            row += 1
                            activation_function_dict[str(activation_func)].append(avg_parameters[1])
                            learning_rate_dict[learning_rate].append(avg_parameters[1])


                            if best_hyperparameter_loss > avg_parameters[0].item():   # if the best calculated loss is greater than the current loss make the current best
                                best_hyperparameter_loss = avg_parameters[0].item()

                                best_hyperparameters = (epoch, learning_rate, layer_no, layer_size, activation_func)
                                print("Best loss till now: ", best_hyperparameter_loss, "And the hyperparameters: ",
                                      best_hyperparameters)

                elif layer_no == 2:  # no hidden layers
                    ml_model = MLModel(layer_no, None, None)
                    ml_model.to(device)
                    optimizer = torch.optim.Adam(ml_model.parameters(), lr=learning_rate)
                    avg_parameters, ml_model = trainer(ml_model, optimizer, epoch)
                    confidence_interval = calculate_confidence_interval(avg_parameters)
                    data[row].append(str("%.2f" % (avg_parameters[1]*100)) +  "% +- " + str("%.2f" % confidence_interval) + "%")

                    row += 1
                    learning_rate_dict[learning_rate].append(avg_parameters[1])



                    if best_hyperparameter_loss > avg_parameters[0].item(): # if the best calculated loss is greater than the current loss make the current best
                        best_hyperparameter_loss = avg_parameters[0].item()
                        print("Here: ", avg_parameters[0].item())
                        best_hyperparameters = (epoch, learning_rate, layer_no, float('-inf'), None)
                        print("Best loss till now: ", best_hyperparameter_loss, "And the hyperparameters: ",
                              best_hyperparameters)

    # plot the graph, print last table and save the model
    finish(data, learning_rates, best_hyperparameter_loss, best_hyperparameters, device, learning_rate_dict,
           activation_function_dict)


if __name__ == '__main__':
    main()
