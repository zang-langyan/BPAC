#include "PDF.h"
#include <iostream>

void PDF::loadFile(std::string __filePath){
  // load pdf file
  doc = poppler::document::load_from_file(__filePath);
  if (!doc) {
    std::cerr << "Could not open file: " << __filePath << std::endl;
    exit(1);
  }

  // set filePath to the last part of the path
  // fileName = __filePath.substr(__filePath.find_last_of("/\\") + 1);
  // set filePath to the full path
  filePath = __filePath;

  // get number of pages
  numPages = doc->pages();

  // set current page to initial value
  currentPage = -1;
}

bool PDF::renderPageImage(int __page){
  if (__page < 0 || __page >= numPages) {
    std::cerr << "Invalid page number: " << __page << std::endl;
    return false;
  }

  // render page
  image = renderer.render_page(
    doc->create_page(__page), // page pointer
    300, // dpi
    300 // dpi
  );

  // save image
  image.save(img_file_name, "jpg");

  return true;
}


// ------- setters -------
void PDF::incrementPage(){
  if (currentPage < numPages - 1) {
    currentPage++;
  }
}

void PDF::decrementPage(){
  if (currentPage > 0) {
    currentPage--;
  }
}

void PDF::setCurrentPage(int __page){
  if (__page >= 0 && __page < numPages) {
    currentPage = __page;
  }
}

// ------- getters -------
int PDF::getNumPages() const{
  return numPages;
}

int PDF::getCurrentPage() const{
  return currentPage;
}

std::string PDF::getFilePath() const{
  return filePath;
}