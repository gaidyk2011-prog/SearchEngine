#include <iostream>
#include <ostream>

#include "SearchServer.h"
#include "JsonConverter.h"
#include "InvertedIndex.h"

int main() {
    JsonConverter converter;
    int max_responses = converter.GetResponsesLimit();
    InvertedIndex invertedIndex;
    SearchServer search(invertedIndex, max_responses);

    invertedIndex.UpdateDocumentBase(converter.GetTextDocuments());

    converter.putAnswers(search.search(converter.GetRequests()));
}
