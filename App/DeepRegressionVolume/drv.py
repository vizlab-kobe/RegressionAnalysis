import tensorflow as tf
from tensorflow import keras
import numpy as np

def main( dimx, dimy, dimz, model_file ):
    # values = np.zeros( dimx * dimy * dimz, dtype = np.float32 )

    # machine learning model
    #model = keras.models.load_model('./DL_s1000_ic1.h5')
    model = keras.models.load_model( model_file )

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
                #values[i*(dimy*dimz)+j*dimz+k] = model.predict(np.array([[x, y, z, 0.0]]), batch_size = 1)[0, 0]
                # values[i*(dimy*dimz)+j*dimz+k] = 0.0

    return values

# -------- debug ---------
#print(main(20,20,20).shape)
