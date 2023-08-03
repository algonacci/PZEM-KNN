import serial
import numpy as np
import pickle
import warnings
warnings.filterwarnings("ignore")

# Meload model KNN dari file pickle
with open('model_knn.pickle', 'rb') as f:
    loaded_model = pickle.load(f)

# labels adalah daftar kelas yang sudah Anda definisikan
labels = ['Kipas Angin & Solder',
          'Kipas Angin & Hair Dryer',
          'Kipas Angin & Blender',
          'Solder & Hair Dryer',
          'Solder & Blender',
          'Blender & Hair Dryer'
          ]

# Buat objek Serial untuk berkomunikasi dengan Arduino
# Ganti 'COM3' sesuai dengan port serial Arduino
ser = serial.Serial('/dev/cu.usbserial-120', 9600)


def receive_data():
    data = ser.readline().decode().strip()
    if data:
        return data
    return None


def main():
    while True:
        data = receive_data()
        if data:
            voltage, current, pf = map(float, data.split(','))

            # Ubah data menjadi numpy array untuk prediksi
            input_data = np.array([[voltage, current]])

            # Lakukan prediksi menggunakan model KNN
            predicted_labels = loaded_model.predict(input_data)
            predicted_classes = [labels[i] for i in predicted_labels]

            # Tampilkan hasil prediksi di layar LCD
            ser.write(
                f"{predicted_classes[0]},{voltage},{current},{pf}".encode())
            print(predicted_classes)


if __name__ == "__main__":
    main()
