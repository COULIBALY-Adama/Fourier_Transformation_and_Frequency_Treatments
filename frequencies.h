//
// Created by coulibaly on 26/05/18.
//

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat ResizeImage (Mat ImageEntree);
Mat FourierTransform (Mat ImageRedimensionnee);
Mat ComputeNorme (Mat ImageTransFourier);
Mat CadransChange (Mat ImageNormeFourier);
Mat TransFourierInverse (Mat ImageTransFourier, int nblignes, int nbcolones);
Mat FiltrePasseBas (Mat ImageTransFourier, float freqcoupure );
Mat FiltrePasseHaut (Mat ImageTransFourier, float freqcoupure );

