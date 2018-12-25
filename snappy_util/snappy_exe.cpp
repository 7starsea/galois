///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "snappy_util.h"

int main(int argc, const char* argv[]){
/*

	///Application headings
	std::cout << "*******************************************************************************" << std::endl;
	std::cout << "*                                                                             *" << std::endl;
	std::cout << "*                          snappy compressor/decompressor                     *" << std::endl;
	std::cout << "*                          Developer : Aimin                                  *" << std::endl;
	std::cout << "*                          Version   : 1.0.0                                  *" << std::endl;
	std::cout << "*                          Date      : 2018-04-17                             *" << std::endl;
	std::cout << "*                                                                             *" << std::endl;
	std::cout << "*******************************************************************************" << std::endl;
*/
	int flag = 0; /// 1 compress; 2 decompress
	try{
		namespace bpo = boost::program_options;
		bpo::options_description desc("Allowed Options:");
		desc.add_options()
			("help,h", "produce help message")
			("compress,c", "compress")
			("decompress,d", "decompress")
			("key,k", bpo::value< std::string >(), "secret key")
			;

		bpo::options_description hidden("Hidden options");
		hidden.add_options()
			("input-file", bpo::value< std::string >(), "input file")
			("output-file", bpo::value< std::string >(), "output file")
			;

		bpo::options_description opts; opts.add(desc).add(hidden);

		bpo::positional_options_description position;
		position.add("input-file", 1);
		position.add("output-file", 1);
		bpo::variables_map vm;
		bpo::store(bpo::command_line_parser(argc, argv).options(opts).positional(position).run(), vm);
		bpo::notify(vm);

		if (vm.count("help")){
			std::cout << desc << std::endl;
			exit(-1);
		}

		if (vm.count("compress")){
			flag |= 1;
		}
		if (vm.count("decompress")){
			flag |= 2;
		}
		
		std::string infile, outfile;
		if (flag > 0){
			if (!(vm.count("input-file"))){
				std::cerr << "You must provide input file!\n";
				exit(-1);
			}

			infile = vm["input-file"].as<std::string>();
			if (!boost::filesystem::is_regular_file(infile)){
				std::cerr << "The input file:" << infile << " does not exist!\n";
				exit(-1);
			}

			if (vm.count("output-file")){
				outfile = vm["output-file"].as<std::string>();
			}
			else{
				outfile = infile + ".bin";
			}
		}
		if (!vm.count("key")){
			std::cerr << "Please provide a secret key for compressing/decompressing with argument --key.\n";
			exit(-1);
		}
		const std::string key = vm["key"].as<std::string>();
		switch (flag)
		{
		case 1:
			if (snappy_compress_to_file(infile, outfile, key)){
				std::cout << ">>> Created compressed file:" << outfile <<" using key: "<< key << ".\n";
			}
			else{
				std::cerr << "There are some error in snappy_compress with key:"<<key<<".\n";
			}
			break;
		case 2:
			if (snappy_uncompress_to_file(infile, outfile, key)){
				std::cout << ">>> Created uncompressed file:" << outfile << " using key: " << key << ".\n";
			}
			else{
				std::cerr << "There are some error in snappy_uncompress with key:" << key << ".\n";
			}
			break;
		case 3:
			std::cerr << "You can not specify both compress and decompress options.\n";
			exit(-1);
			break;
		default:
			std::cout << desc << std::endl;
			break;
		}

	}
	catch (std::exception &e){
		std::cout << "===> ERROR: " << e.what() << std::endl;
		exit(-1);
	}


	return 0;
}
