#include "PDF_Information.h"

void Information::loadFile(std::string __filePath) {
  doc = poppler::document::load_from_file(__filePath);
  if (!doc) {
    std::cout << "Error loading file" << std::endl;
    return;
  }
  
  filePath = __filePath;
  // get meta data
  title = doc->get_title().to_latin1();
  author = doc->get_author().to_latin1();
  // title = std::string(doc->get_title().to_utf8().begin(), doc->get_title().to_utf8().end());
  // author = std::string(doc->get_author().to_utf8().begin(), doc->get_author().to_utf8().end());

  time_t time = doc->get_creation_date_t();
  tm *ltm = gmtime(&time);
  year = 1900 + ltm->tm_year;

  citekey = author + std::to_string(year);
}

// ------- getters -------
std::string Information::getFilePath() {
  return filePath;
}

std::string Information::getTitle() {
  return title;
}

std::string Information::getAuthor() {
  return author;
}

std::string Information::getJournal() {
  return journal;
}

int Information::getYear() {
  return year;
}

std::string Information::getPublisher() {
  return publisher;
}

std::string Information::getNotes() {
  return notes;
}

std::string Information::getCitekey() {
  return citekey;
}

File_Info Information::getFileInfo() {

  return File_Info{
    type,
    title,
    author,
    journal,
    year,
    publisher,
    notes,
    citekey
  };

}
