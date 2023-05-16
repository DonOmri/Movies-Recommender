#include "RecommenderSystemLoader.h"

using std::runtime_error;
using std::range_error;

unique_ptr<RecommenderSystem>
    RecommenderSystemLoader::create_rs_from_movies_file  (const string
    &movies_file_path) noexcept (false)
{
  ifstream input = file_opener (movies_file_path);
  RecommenderSystem new_sys;
  string name, year, feats;

  //for each line in file, create a movie with correspondent features vector
  while(getline (input,name,NAME_DELIM)) //split input to name
  {
    try
    {
      //split input to year (as string) and feats (as string)
      getline (input,year,YEAR_DELIM);
      getline (input,feats,FEATS_DELIM);

      vector<double> features = create_feat_vec(feats);
      new_sys.add_movie (name,stoi(year),features);
    }
    catch(const range_error& err)
    {
      input.close();
      throw err;
    }
    catch(const runtime_error&)
    {
      input.close();
      throw;
    }
  }

  input.close();
  return make_unique<RecommenderSystem>(new_sys);
}

/***************************
RecommenderSystemLoader private(helper) functions
***************************/

vector<double> RecommenderSystemLoader::create_feat_vec(const string& str)
{
  istringstream feats_stream(str);
  vector<double> new_vec;
  string new_feat;

  while(getline(feats_stream,new_feat,INNER_FEATS_DELIM))
  {
    double new_feat_as_num = stod(new_feat);
    if(new_feat_as_num > MAX_RTG || new_feat_as_num < MIN_RTG)
    {
      throw range_error(RNG_ERR_MSG);
    }
    new_vec.push_back(new_feat_as_num);
  }
  return new_vec;
}

ifstream RecommenderSystemLoader::file_opener(const string& movies_file_path)
noexcept (false)
{
  ifstream input(movies_file_path);
  if(!input.is_open())
  {
    throw runtime_error(PATH_ERR_MSG);
  }
  return input;
}