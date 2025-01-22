//
// Created by marijn on 1/20/25.
//

#ifndef FLIB_SERIALIZER_HPP
#define FLIB_SERIALIZER_HPP


class serializer {
	virtual void load(file_t file) = 0;
	virtual void save(file_t file) = 0;
};


#endif //FLIB_SERIALIZER_HPP
