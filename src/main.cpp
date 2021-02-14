
#include <iostream>
#include <image.h>



void PrintUsage() {
    std::cout << "itoa [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-h | --help        print help" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        PrintUsage();
    }

    // Print out instructions on how to use.
    if (argc < 2) {
        std::cerr << "error in calling program" << std::endl;
        return 1;
    }

    img::image image(argv[1]);
    img::image grayscale = image.to_grayscale();
    grayscale.save();
//
//    for(int i(0) ; i < image.getSize().x ; i++) // converts each pixel to grayscale with weights
//    {
//        for(int j(0) ; j < image.getSize().y ; j++)
//        {
//            bwImage.setPixel(i,j,Color::Color(  image.getPixel(i,j).r*RED_COEFF + image.getPixel(i,j).g*GREEN_COEFF + image.getPixel(i,j).b*BLUE_COEFF ,
//                                                image.getPixel(i,j).r*RED_COEFF + image.getPixel(i,j).g*GREEN_COEFF + image.getPixel(i,j).b*BLUE_COEFF ,
//                                                image.getPixel(i,j).r*RED_COEFF + image.getPixel(i,j).g*GREEN_COEFF + image.getPixel(i,j).b*BLUE_COEFF) ) ;
//        }
//    }
//    cout<<"Done !"<<endl;
//    cout<<"Conversion to low resolution..."<<endl;
//    int Yresolution = 2*Xresolution ; // letters spacing in notePad works best with these settings
//    Image bwLowResImage(image) ;
//    int grey = 0 ;
//    for(int i(0) ; i < image.getSize().x-Xresolution ; i+=Xresolution) // converting to a lower resolution if the users want less char in his final .txt : pixels are averaged in squares of Xresolution*Yresolution
//    {
//        for(int j(0) ; j < image.getSize().y-Yresolution ; j+=Yresolution)
//        {
//            for(int x(0) ; x < Xresolution ; x++)
//            {
//                for(int y(0) ; y < Yresolution ; y++)
//                {
//                    grey += bwImage.getPixel(i+x,j+y).r ;
//                }
//            }
//            grey /= Xresolution * Yresolution + 1;
//            for(int x(0) ; x < Xresolution ; x++)
//            {
//                for(int y(0) ; y < Yresolution ; y++)
//                {
//                    bwLowResImage.setPixel(i+x,j+y,Color( grey ,grey , grey ) ) ;
//                }
//            }
//        }
//    }
//    cout<<"Done !"<<endl;
//    cout<<"Conversion to ASCII..."<<endl;
//    string asciiArt ;
//
//    for(int j(0) ; j < bwLowResImage.getSize().y ; j+=Yresolution)
//    {
//        for(int i(0) ; i < bwLowResImage.getSize().x ; i+=Xresolution)
//        {
//            asciiArt += getCorrespondingAsciiChar(bwLowResImage.getPixel(i,j).r,among) ;
//        }
//        asciiArt+="\n" ;
//    }
//    ofstream output(outputPath) ;
//    if(output) output << asciiArt ;
//    cout<<"Finished ! You will find the result at : "<<outputPath<<endl;
//    system("PAUSE") ;
    return 0 ;
}