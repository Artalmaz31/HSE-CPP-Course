#include "search.h"
#include <cmath>

bool is_line_separator(char c) {
    return c == '\n';
}
bool is_word_separator(char c) {
    return !std::isalpha(static_cast<unsigned char>(c));
}

std::vector<std::string_view> SplitString(std::string_view line, bool (*is_separator)(char)) {
    std::vector<std::string_view> words;
    size_t start = 0;
    while (start < line.length()) {
        while (start < line.length() && is_separator(line[start])) {
            ++start;
        }
        if (start == line.length()) {
            break;
        }
        size_t end = start;
        while (end < line.length() && !is_separator(line[end])) {
            ++end;
        }
        words.push_back(line.substr(start, end - start));
        start = end;
    }
    return words;
}

bool AreStringsEqualCaseInsensitive(std::string_view a, std::string_view b) {
    return std::equal(
        a.begin(), a.end(),
        b.begin(), b.end(),
        [](unsigned char x, unsigned char y) {
            return std::tolower(x) == std::tolower(y);
        }
    );
}

bool CompareStringsCaseInsensitive(std::string_view a, std::string_view b) {
    return std::lexicographical_compare(
        a.begin(), a.end(),
        b.begin(), b.end(),
        [](unsigned char x, unsigned char y) { 
            return std::tolower(x) < std::tolower(y); 
        }
    );
};

class Document {
public:
    Document() {}
    Document(size_t index, std::string_view content) : index(index), content(content) {
        words = SplitString(this->content, &is_word_separator);
    }
    size_t index = 0;
    std::string_view content = "";
    std::vector<std::string_view> words = {};
    double tfidf = 0.0;

    size_t CountOccurrences(std::string_view term) const {
        size_t counter = 0;
        for (std::string_view word : words) {
            if (AreStringsEqualCaseInsensitive(word, term)) {
                ++counter;
            }
        }
        return counter;
    }
};

double CalculateTF(std::string_view term, const Document& doc) {
    size_t counter = doc.CountOccurrences(term);
    return doc.words.size() > 0 ? static_cast<double>(counter) / doc.words.size() : 0;
}

double CalculateIDF(std::string_view term, const std::vector<Document>& docs) {
    size_t counter = 0;
    for (const Document& doc : docs) {
        if (doc.CountOccurrences(term)) {
            ++counter;
        }
    }
    return counter > 0 ? std::log(docs.size() / static_cast<double>(counter)) : 0;
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    if (text.empty() || query.empty() || results_count == 0) {
        return {};
    }

    std::vector<std::string_view> docs_raw = SplitString(text, &is_line_separator);
    std::vector<Document> docs;
    for (size_t i = 0; i < docs_raw.size(); ++i) {
        Document doc(i, docs_raw[i]);
        if (!doc.words.empty()) {
            docs.push_back(doc);
        }
    }

    std::vector<std::string_view> query_words = SplitString(query, &is_word_separator);
    std::sort(query_words.begin(), query_words.end(), CompareStringsCaseInsensitive);
    query_words.erase(
        std::unique(query_words.begin(), query_words.end(), AreStringsEqualCaseInsensitive),
        query_words.end()
    );

    for (std::string_view word : query_words) {
        double idf = CalculateIDF(word, docs);
        for (Document& doc : docs) {
            double tf = CalculateTF(word, doc);
            doc.tfidf += tf * idf;
        }
    }

    std::sort(
        docs.begin(), docs.end(),
        [](const Document& a, const Document& b) {
            if (std::abs(a.tfidf - b.tfidf) < 1e-6) {
                return a.index < b.index;
            }
            return a.tfidf > b.tfidf;
        }
    );

    std::vector<std::string_view> result;
    for (const Document& doc : docs) {
        if (doc.tfidf > 0) {
            result.push_back(docs_raw[doc.index]);
        } else {
            break;
        }
        if (result.size() == results_count) {
            break;
        }
    }
    return result;
}
