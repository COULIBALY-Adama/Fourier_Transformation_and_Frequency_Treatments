//
// Created by coulibaly on 26/05/18.
//


#include "frequencies.h"
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


int main( int argc, char** argv )
{
    int choix;
    float frec;
    if(argc!= 2 ){
        cout << "Please follow the following syntax:./Filters image_name "<<endl;
    }
    else
    {
        cout << "Please choose the type of filter to use"<<endl;
        cout << "Enter 1 for the low pass filter"<<endl;
        cout << "Enter 2 for the high pass filter"<<endl;
        cout << "choice :";
        cin >> choix;
        cout << "Please enter the cutoff frequency (a float number between 0 and 1) :";
        cin >> frec;
        const char* NomImage = argv[1];

        // Ouverture de l'image en nuance de gris
        Mat ImageEntry = imread(NomImage, CV_LOAD_IMAGE_GRAYSCALE);

        if(!ImageEntry.data){
            cout << "Please provide a valid image"<<endl;
            return -1;
        }
        else{

            // Calcul de la transformée de fourier de l'image

            Mat ImageTransFourier = FourierTransform(ResizeImage(ImageEntry));

            Mat ImageTransTraitee;

            //Application du filtre suivant le choix
            if(choix == 1){
                ImageTransTraitee = FiltrePasseBas(ImageTransFourier, frec);
            }
            else if(choix == 2){
                ImageTransTraitee = FiltrePasseHaut(ImageTransFourier, frec);
            }

            //Détermination de la norme du spectre
            Mat InitialSpectrum = CadransChange(ComputeNorme(ImageTransFourier));
            Mat SpectrumTreatment =  ComputeNorme(ImageTransTraitee);

            Mat ReverseTransImage = TransFourierInverse(CadransChange(ImageTransTraitee),ImageEntry.rows, ImageEntry.cols);

            //Affichage des différentes images
            imshow("Image Entry", ImageEntry);
            imshow("Initial Fourier Spectrum", InitialSpectrum);
            imshow("Processed Fourier Spectrum", SpectrumTreatment);
            imshow("Image After Processing", ReverseTransImage);

            // Enregistrement de l'image originale
            if(!imwrite("Image_Entry.png", ImageEntry))
                cout<<"Error while saving"<<endl;

            // Enregistrement de l'image du spectre initial
            if(!imwrite("Initial_Spectrum.png", InitialSpectrum))
                cout<<"Error while saving"<<endl;

            // Enregistrement de l'image du spectre filtré
            if(!imwrite("Processed_spectrum.png", SpectrumTreatment))
                cout<<"Erreur lors de l'enregistrement"<<endl;

            // Enregistrement de l'image de la transformée inverse de fourier du filtrage
            if(!imwrite("Image_Restored.png", ReverseTransImage))
                cout<<"Error while saving"<<endl;

            waitKey();

            return 0;

        }
    }
}

