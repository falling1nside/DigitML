
#include "dataset.hpp"
#include "NN.hpp"
#include "../lib/matrix.h"
#include <vector>
#include <iostream>

void debug(Example e) {
    static std::string shades = " .:-=+*#%@";
    for (unsigned int i = 0; i < 28 * 28; i++) {
        if (i % 28 == 0) printf("\n");
        printf("%c", shades[e.data[i] / 30]);
    }
    printf("\nLabel: %d\n", e.label);
}

std::vector<double> load_matrix(Example& e) {
    std::vector<double> result(e.data, e.data + 28 * 28);
    return result;
}

const double calculate_accuracy(const Matrix<unsigned char>& images, const Matrix<unsigned char>& labels, NeuralNetwork n) {
  unsigned int correct = 0;
  for (unsigned int i = 0; i < images.rows(); ++i) {
    Example e;
    for (int j = 0; j < 28*28; ++j) {
        e.data[j] = images[i][j];
    }
    e.label = labels[i][0];
    unsigned int guess = n.compute(e);
    if (guess == (unsigned int)e.label) correct++;
  }
  const double accuracy = (double)correct/images.rows();

  return accuracy;
}

#ifdef TESTS
#include <gtest/gtest.h>

    NeuralNetwork n;
    TEST(TanhTest, ZeroInput) {
        std::vector<double> result = n.tanh({ 0.0 });
        EXPECT_NEAR(result[0], 0.0, 1e-9);
    }

    TEST(TanhTest, PositiveInput) {
        std::vector<double> result = n.tanh({ 1.0 });
        EXPECT_NEAR(result[0], std::tanh(1.0), 1e-9);
    }

    TEST(TanhTest, NegativeInput) {
        std::vector<double> result = n.tanh({ -1.0 });
        EXPECT_NEAR(result[0], std::tanh(-1.0), 1e-9);
    }

    TEST(TanhTest, LargeValues) {
        std::vector<double> result = n.tanh({ 5.0 });
        EXPECT_NEAR(result[0], 0.9999092, 1e-7);
    }

    TEST(TanhTest, MultipleValues) {
        std::vector<double> result = n.tanh({ -2.0, 0.0, 2.0 });
        EXPECT_NEAR(result[0], std::tanh(-2.0), 1e-9);
        EXPECT_NEAR(result[1], 0.0, 1e-9);
        EXPECT_NEAR(result[2], std::tanh(2.0), 1e-9);
    }

NeuralNetwork n;

TEST(FunctionTesting, test_bent_identity) {
  std::vector<double> t1 = {0};
  EXPECT_NEAR(n.bent_identity(t1)[0], 0, 1e-4);
}

TEST(FunctionTesting, test_sigmoid_incr) {  
    std::vector<double> t1 = {-10, 0, 10};
    std::vector<double> t2 = {-5.4750621894395549, 0, 14.5249378105604451};
    EXPECT_EQ(n.bent_identity(t1), t2);
}

TEST(FunctionTesting, test_sigmoid_decr) {
    Matrix<unsigned char> images_test(0, 0);
    Matrix<unsigned char> labels_test(0, 0);
    load_dataset(images_test, labels_test, "data/t10k-images-idx3-ubyte", "data/t10k-labels-idx1-ubyte");
    const unsigned int num_iterations = 5;
    EXPECT_GT(calculate_accuracy(images_test, labels_test, n), 0.01);
}

TEST(FunctionTesting, test_throw) {
    const unsigned int num_iterations = 5;
    Matrix<unsigned char> images_train(0, 0);
    Matrix<unsigned char> labels_train(0, 0);
    load_dataset(images_train, labels_train, "data/train-images-idx3-ubyte", "data/train-labels-idx1-ubyte");
    EXPECT_NO_THROW(n.train(num_iterations, images_train, labels_train));
}

TEST(FunctionTesting, test_sigmoid_comp) {  
  std::vector<double> t1 = {-10};
  EXPECT_TRUE(n.sigmoid(t1)>n.bent_identity(t1));
}

#endif

int main(int argc, char **argv) {
    std::cout << "Choose activation function (1 - Sigmoid, 2 - Tanh): ";
    int choice;
    std::cin >> choice;

    if (choice == 2) {
    #define USE_TANH
        std::cout << "Using Tanh activation function" << std::endl;
    }
    else {
        std::cout << "Using Sigmoid activation function" << std::endl;
    }

    #ifdef TEST_TANH
    run_tanh_tests(); // ������ ����� ������
    return 0;
    #endif

    #ifdef TEST
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    #endif
    
    Matrix<unsigned char> images_train(0, 0);
    Matrix<unsigned char> labels_train(0, 0);
    load_dataset(images_train, labels_train, "data/train-images-idx3-ubyte", "data/train-labels-idx1-ubyte");

    Matrix<unsigned char> images_test(0, 0);
    Matrix<unsigned char> labels_test(0, 0);
    load_dataset(images_test, labels_test, "data/t10k-images-idx3-ubyte", "data/t10k-labels-idx1-ubyte");

    NeuralNetwork n;

    // Tests to see that data was read in properly
    /*for (int i = 0; i < 10; ++i) {
        Example e;
        for (int j = 0; j < 28*28; ++j) {
            e.data[j] = images_train[i][j];
        }
        e.label = labels_train[i][0];
        debug(e);
        printf("Guess: %d\n", n.compute(e));
    }
    for (int i = 0; i < 10; ++i) {
        Example e;
        for (int j = 0; j < 28*28; ++j) {
            e.data[j] = images_test[i][j];
        }
        e.label = labels_test[i][0];
        debug(e);
        printf("Guess: %d\n", n.compute(e));
    }*/
    const unsigned int num_iterations = 5;
    n.train(num_iterations, images_train, labels_train);

    const double accuracy_train = calculate_accuracy(images_train, labels_train, n);
    const double accuracy_test = calculate_accuracy(images_test, labels_test, n);

    printf("Accuracy on training data: %f\n", accuracy_train);
    printf("Accuracy on test data: %f\n", accuracy_test);

    return 0;
}
