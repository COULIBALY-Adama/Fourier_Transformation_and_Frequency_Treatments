//
// Created by coulibaly on 26/05/18.
//

#include "frequencies.h"

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Function to resize the input image



Mat ResizeImage (Mat Image_Entry){

    Mat padded; // resize image to return on output

    // optimal resized image size
    int m = getOptimalDFTSize( Image_Entry.rows );
    int n = getOptimalDFTSize( Image_Entry.cols );

    // Added padding value in the resized image.
    copyMakeBorder(Image_Entry, padded, 0, m - Image_Entry.rows, 0, n - Image_Entry.cols, BORDER_CONSTANT, Scalar::all(0));
    return padded;

}

Mat FourierTransform (Mat Resized_Image){


    // Creation of two Containers with floating point values for the real and imaginary parts of the fourier transform.
    Mat Containers[] = {Mat_<float>(Resized_Image), Mat::zeros(Resized_Image.size(), CV_32F)};

    // Concatenation of the two containers.
    Mat ImageTransFourier;
    merge(Containers, 2, ImageTransFourier);

    // Fourier transform.
    dft(ImageTransFourier, ImageTransFourier);

    return ImageTransFourier;
}

Mat ComputeNorme (Mat ImageTransFourier){


    vector<Mat> Containers;
    // separation of real and imaginary parts.
    split(ImageTransFourier, Containers);

    // Calculation of the norm of the Fourier transform.
    magnitude(Containers[0], Containers[1], Containers[0]);
    Mat ImageNormeFourier = Containers[0];

    // Switching to the logarithmic scale.
    ImageNormeFourier += Scalar::all(1);
    log(ImageNormeFourier, ImageNormeFourier);


    // spectrum cropping
    ImageNormeFourier = ImageNormeFourier(Rect(0, 0, ImageNormeFourier.cols & -2, ImageNormeFourier.rows & -2));
    normalize(ImageNormeFourier, ImageNormeFourier, 0, 1, CV_MINMAX);
    Mat Sortie;
    ImageNormeFourier.convertTo(Sortie, CV_8UC1,255);

    return Sortie;
}

Mat CadransChange (Mat ImageNormeFourier){

    int cx = ImageNormeFourier.cols/2;
    int cy = ImageNormeFourier.rows/2;

    Mat cadran0(ImageNormeFourier, Rect(0, 0, cx, cy));   // Cadrant en haut à gauche
    Mat cadran1(ImageNormeFourier, Rect(cx, 0, cx, cy));  // Cadrant en haut à droite
    Mat cadran2(ImageNormeFourier, Rect(0, cy, cx, cy));  // Cadrant en bas à gauche
    Mat cadran3(ImageNormeFourier, Rect(cx, cy, cx, cy)); // Cadrant en bas à droite

    Mat tmp;                           // tampon pour l echange des cadrants

    //Echange entre le Cadrant en haut à gauche et le Cadrant en bas à droite
    cadran0.copyTo(tmp);
    cadran3.copyTo(cadran0);
    tmp.copyTo(cadran3);

    // Exchange between the Dial(cadran) at the top right and the Dial(cadran) at the bottom left.
    cadran1.copyTo(tmp);
    cadran2.copyTo(cadran1);
    tmp.copyTo(cadran2);

    return ImageNormeFourier;

}

Mat TransFourierInverse (Mat ImageTransFourier, int nblignes, int nbcolones){
    Mat ImageRestore;
    Mat ImageFourierInverse;
    vector<Mat> Containers;

    // Inverse Fourier transform.
    dft( ImageTransFourier,ImageFourierInverse,DFT_INVERSE + DFT_SCALE);

    split(ImageFourierInverse,Containers);

    ImageRestore = Containers[0];

    // Cropping the image
    ImageRestore = ImageRestore(Rect(0, 0, ImageRestore.cols & -2, ImageRestore.rows & -2));

    // Converting to Grayscale Image
    ImageRestore.convertTo(ImageRestore, CV_8UC1);
    Mat Sortie (ImageRestore, Rect(0,0, nbcolones, nblignes));

    return Sortie;
}

Mat FiltrePasseBas (Mat ImageTransFourier, float freqcoupure ){
    Mat ImageFiltrePasseBas;
    vector<Mat> Containers;
    float cx = ImageTransFourier.cols / (float) 2;
    float cy = ImageTransFourier.rows / (float) 2;
    // Determining the radius of the circle
    int rayon = (int) (min(float(cx), float(cy)))*freqcoupure;

    split(ImageTransFourier,Containers);

    // Update values outside the circle
    for(int k = 0; k < (int) Containers.size(); k++){
        CadransChange(Containers[k]);
        for(int i = 0; i < Containers[k].rows; i++)
            for(int j = 0; j < Containers[k].cols; j++)
            {
                if(pow((j - cx),2) + pow((i - cy),2) > pow(rayon,2))
                    Containers[k].at<float>(i, j) = 0;
            }
    }
    merge(Containers,ImageFiltrePasseBas);


    return ImageFiltrePasseBas;

}

Mat FiltrePasseHaut (Mat ImageTransFourier, float freqcoupure ){
    Mat ImageFiltrePasseHaut;
    vector<Mat> Containers;
    float cx = ImageTransFourier.cols / (float) 2;
    float cy = ImageTransFourier.rows / (float) 2;
    // Determining the radius of the circle
    int rayon = (int) (min(float(cx), float(cy)))*freqcoupure;

    split(ImageTransFourier,Containers);
    // Updating values inside the circle
    for(int k = 0; k < (int) Containers.size(); k++){
        CadransChange(Containers[k]);
        for(int i = 0; i < Containers[k].rows; i++)
            for(int j = 0; j < Containers[k].cols; j++)
            {
                if(pow((j - cx),2) + pow((i - cy),2) < pow(rayon,2))
                    Containers[k].at<float>(i, j) = 0;
            }
    }
    merge(Containers,ImageFiltrePasseHaut);

    return ImageFiltrePasseHaut;

}

