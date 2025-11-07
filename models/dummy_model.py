import tensorflow as tf
import numpy as np

x = np.random.rand(100, 3).astype(np.float32)
y = np.sum(x, axis=1, keepdims=True)

model = tf.keras.Sequential([
    tf.keras.layers.Dense(4, activation='relu', input_shape=(3,)),
    tf.keras.layers.Dense(1)
])

model.compile(optimizer='adam', loss='mse')
model.fit(x, y, epochs=30, verbose=0)

converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()
open("models/simple_model.tflite", "wb").write(tflite_model)