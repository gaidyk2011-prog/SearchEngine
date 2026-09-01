#include "../include/SearchServer.h"
#include "../include/JsonConverter.h"
#include "../include/InvertedIndex.h"

int main() {
    JsonConverter converter;
    int max_responses = converter.GetResponsesLimit();
    InvertedIndex invertedIndex;
    SearchServer search(invertedIndex, max_responses);

    invertedIndex.UpdateDocumentBase(converter.GetTextDocuments());

    converter.putAnswers(search.search(converter.GetRequests()));
}
