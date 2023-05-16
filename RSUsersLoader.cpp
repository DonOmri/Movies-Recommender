#include "RSUsersLoader.h"
#include "RecommenderSystemLoader.h"


std::vector <RSUser> RSUsersLoader::create_users_from_file (const
std::string &users_file_path, unique_ptr <RecommenderSystem> rs) noexcept
(false)
{
  ifstream input = RecommenderSystemLoader::file_opener (users_file_path);

  //insert given movies into a vector
  string str_movies;
  getline(input,str_movies);
  auto movies_vec = create_movies_vector(str_movies,*rs);
  vector<RSUser> users_vec;

  //add users and their ranked movies;
  shared_ptr<RecommenderSystem> shared_rs = move(rs);
  string user_data;
  while(getline(input,user_data,END_OF_USER_DATA))
  {
    try
    {
      auto rs_of_user = shared_rs;
      istringstream user_data_stream(user_data);
      users_vec.push_back (create_user(rs_of_user,user_data_stream,
                                       movies_vec));
    }
    catch(const runtime_error&)
    {
      input.close();
      throw;
    }
  }

  input.close();
  return users_vec;
}

/***************************
RSUsersLoader private(helper) functions
***************************/

vector<sp_movie> RSUsersLoader::create_movies_vector(string& movies_str,
                                      const RecommenderSystem& rs)
{
  vector<sp_movie> movies_vec;
  replace(movies_str.begin(), movies_str.end(), REPLACE_THIS, WITH_THIS);
  string name_and_year, name, year;
  istringstream s(movies_str);

  while(s>>name>>year)
  {
    movies_vec.push_back (rs.get_movie (name, stoi (year)));
  }

  return movies_vec;
}


RSUser RSUsersLoader::create_user(shared_ptr<RecommenderSystem>& rs,
                                  istream& details, const vector<sp_movie>&
                                  movies_vec)
{
  rank_map new_user_rm(movies_vec.size(), sp_movie_hash, sp_movie_equal);
  string rank, name;
  getline(details,name,CUT_TO_GET_USER_NAME);

  //insert the ranked movies into the user rank map
  for(const auto& movie:movies_vec )
  {
    getline(details,rank,CUT_TO_GET_MOVIE_RATE);

    if(isdigit(rank[NA_TEST_IDX]) && stod(rank) >= MIN_RTG && stod(rank) <=
    MAX_RTG)
    {
      new_user_rm[movie] = stod(rank);
    }
    else if (isdigit(rank[NA_TEST_IDX]) || rank[NA_TEST_IDX]  == IS_NEGATIVE)
    {
      throw runtime_error(ILLEGAL_RTG_MSG);
    }
  }

  RSUser new_user(new_user_rm,name,rs);
  return new_user;
}