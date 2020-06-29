import tensorflow as tf
from tensorflow import keras
import numpy as np

def main( dimx, dimy, dimz, model_file ):
    # machine learning model
    model = keras.models.load_model( model_file )
    model._make_predict_function()

    # NN solution
    values = np.zeros(dimx * dimy * dimz, dtype = np.float32)
    index = 0
    for i in range(dimx):
        x = i * (1. / (dimx-1))
        for j in range(dimy):
            y = j * (1. / (dimy-1))
            for k in range(dimz):
                z = k * (1. / (dimz-1))
                values[index] = model.predict(np.array([[x, y, z, 0.0]]), batch_size = 1)[0, 0]
                index = index + 1

    return values
