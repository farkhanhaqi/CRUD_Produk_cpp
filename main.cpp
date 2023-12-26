#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

struct Produk{
    int pk;
    char nama[20];
    int harga;
};

int pilihMenu();
void checkDatabase(fstream &data);
void writeData(fstream &data, int posisi,Produk &inputProduk)
{
    data.seekp((posisi-1)*sizeof(Produk), ios::beg);
    data.write(reinterpret_cast<char*>(&inputProduk), sizeof(Produk));
}

int getDataSize(fstream &data)
{
    int start, end;
    data.seekg(0, ios::beg);
    start = data.tellg();
    data.seekg(0, ios::end);
    end = data.tellg();

    return (end - start) / sizeof(Produk);
}

Produk readData(fstream &data, int posisi)
{
    Produk readProduk;

    data.seekp((posisi-1)*sizeof(Produk), ios::beg);
    data.read(reinterpret_cast<char*>(&readProduk), sizeof(Produk));

    return readProduk;
}

void addDataProduk(fstream &data)
{
    Produk inputProduk, lastProduk;
    int size;
    size = getDataSize(data);
    if(size == 0) {
        inputProduk.pk = 1;
    } else {
        lastProduk = readData(data, size);
        inputProduk.pk = lastProduk.pk + 1;
    }

    cout << "jumlah produk yang sudah diinput : " << size << endl;
    cout << "pk sekarang adalah : " << inputProduk.pk<< endl;

    cout << "nama produk : ";
    cin.getline(inputProduk.nama, 20);
    cout << "harga produk : ";
    cin >> inputProduk.harga;

    writeData(data, size+1 , inputProduk);
}

void displayDataProduk(fstream &data)
{
    Produk showProduk;
    int size;
    size = getDataSize(data);

    cout << "No. \tpk \tNama \tHarga" << endl;
    for(int i = 1; i <= size; i++)
    {
        showProduk = readData(data, i);
        cout << i << "\t";
        cout << showProduk.pk << "\t";
        cout << showProduk.nama << "\t";
        cout << showProduk.harga << endl;
    }
}

void updateDataProduk(fstream &data)
{
    Produk updateProduk;
    int nomor;

    cout << "masukkan no, produk yang ingin dirubah:";
    cin >> nomor;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Data yang akan dirubah :";
    updateProduk = readData(data, nomor);
    cout << "nama :" << updateProduk.nama << endl;
    cout << "harga:" << updateProduk.harga << endl;

    cout << "Proses perubahan data produk";
    cout << "nama produk :";
    cin.getline(updateProduk.nama, 20);
    cout << "harga produk :";
    cin >> updateProduk.harga;

    writeData(data, nomor, updateProduk);
}

void deleteDataProduk(fstream &data)
{
    Produk blankProduk, temporaryProduk;
    int size, nomor, offset;
    fstream dataSementara;

    size = getDataSize(data);
    blankProduk.pk = 0;
    cout << "pilih nama produk yang akan dihapus : ";
    cin >> nomor;
    writeData(data, nomor, blankProduk);
    offset = 0;
    dataSementara.open("temp,dat", ios::trunc | ios::in |ios::binary);
    for(int i = 1; i <= size; i++){
        temporaryProduk = readData(data, i);
        if(temporaryProduk.pk != 0) {
                writeData(dataSementara, i , temporaryProduk);
        } else {
            offset = offset + 1;
        }
    }

    data.close();
    data.open("data.bin", ios::trunc | ios::out | ios::binary);
    data.close();
    data.open("data.bin", ios::out | ios::in | ios::binary);

    size = getDataSize(dataSementara);
    for(int i = 1; i <= size; i++)
    {
        temporaryProduk = readData(dataSementara, i);
        writeData(data, i, temporaryProduk);
    }
}

int main()
{
    fstream data;
    checkDatabase(data);

    int pilihan = pilihMenu();
    char is_continue;

    while(pilihan != 5)
    {
        switch(pilihan)
        {
            case 1:
                cout << "Menu Tambah Produk" << endl;
                addDataProduk(data);
                break;
            case 2:
                cout << "Menu Lihat Produk" << endl;
                displayDataProduk(data);
                break;
            case 3:
                cout << "Menu Ubah Data Produk" << endl;
                displayDataProduk(data);
                updateDataProduk(data);
                displayDataProduk(data);
                break;
            case 4:
                cout << "Menu Hapus Data Produk" << endl;
                displayDataProduk(data);
                deleteDataProduk(data);
                displayDataProduk(data);
                break;
            default:
                cout << "Menu Tidak Valid" << endl;
                break;
        }

        label_continue:
        cout << "Lanjutkan?[y/n] : ";
        cin >> is_continue;
        if(is_continue == 'y' || is_continue == 'Y') {
            pilihan = pilihMenu();
        } else if(is_continue == 'n' || is_continue == 'N') {
            break;
        } else {
            goto label_continue;
        }
    }




    return 0;
}

int pilihMenu()
{
 //system("cls");
 int input;
    cout << "==> Program Kelola Produk <==" << endl;
    cout << "=============================" << endl;
    cout << "1. Tambah Data Produk" << endl;
    cout << "2. Lihat Data Produk" << endl;
    cout << "3. Ubah Data Produk" << endl;
    cout << "4. Hapus Data Produk" << endl;
    cout << "5. Selesai" << endl;
    cout << "=============================" << endl;
    cout << "Pilih Menu?[1-5]";
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return input;
}

void checkDatabase(fstream &data)
{
    data.open("data.bin", ios::out | ios::in | ios::binary);
    if(data.is_open()) {
        cout << "database ditemukan" << endl;
    } else {
        cout << "database belum ada, bikin baru" << endl;
        data.close();
        data.open("data.bin", ios::trunc | ios::out | ios::in | ios::binary);
    }
}
