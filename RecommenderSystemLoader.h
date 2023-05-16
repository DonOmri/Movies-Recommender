#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H

#include "RecommenderSystem.h"
#include <fstream>
#include <sstream>

#define PATH_ERR_MSG "Error: invalid file path."
#define RNG_ERR_MSG "Error: a feature rank is a number between 1 to 10."
#define NAME_DELIM '-'
#define YEAR_DELIM ' '
#define FEATS_DELIM '\n'
#define INNER_FEATS_DELIM ' '
#define MAX_RTG 10
#define MIN_RTG 1

using std::ifstream;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::istringstream;


class RecommenderSystemLoader {

 private:
  /**
   * creates the deatures vector for a single movie
   * @param str the feats as a string, seperated by " " each
   * @return vector containing the feats as doubles
   */
  static vector<double> create_feat_vec(const string& str);

  /**
   * loads the path file, and throws an error if invalid
   * @param movies_file_path
   * @return an ifstream object containing the file content
   */
  static ifstream file_opener (const string &movies_file_path);

  friend class RSUsersLoader;

 public:
  RecommenderSystemLoader () = delete;

  /**
   * loads movies by the given format for movies with their feature's score
   * @param movies_file_path a path to the file of the movies
   * @return smart pointer to a RecommenderSystem which was created with
   * those movies
   */
  static unique_ptr<RecommenderSystem> create_rs_from_movies_file
	  (const std::string &movies_file_path) noexcept (false);
};

#endif //RECOMMENDERSYSTEMLOADER_H
