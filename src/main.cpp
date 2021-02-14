
#include <iostream>
#include <image.h>

void usage() {
    std::cout << "img [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-h | --help        print help" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        usage();
    }

    // Print out instructions on how to use.
    if (argc < 2) {
        std::cerr << "error in calling program" << std::endl;
        return 1;
    }

    img::image image(argv[1]);

    image.to_grayscale().save();
    image.to_lower_resolution(120, 60).save();


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