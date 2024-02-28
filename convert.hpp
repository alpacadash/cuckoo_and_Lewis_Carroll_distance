#ifndef ___PROJECT_3_CONVERT_HPP
#define ___PROJECT_3_CONVERT_HPP

#include <istream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>

#include "Wordset.hpp"

template <unsigned int H1, unsigned int H2, unsigned int EvictThreshold>
[[nodiscard]] std::vector<std::string> convert(const std::string& start,
                                               const std::string& end,
                                               const BaseWordSet<H1, H2, EvictThreshold>& words) {
    // TODO: Implement this. The stub is only to allow the code to remove
    //       (you can remove it).
    if (start == end) return {};
    if (!words.contains(start) || !words.contains(end)) return {};

    std::queue<std::string> visit;
    std::unordered_map<std::string, std::string> pred;
    std::unordered_set<std::string> visited;

    visit.push(start);
    visited.insert(start);

    while (!visit.empty()) {
        std::string current = visit.front();
        visit.pop();

        for (size_t i = 0; i < current.length(); ++i) {
            std::string transformed = current;
            for (char c = 'a'; c <= 'z'; ++c) {
                transformed[i] = c;
                if (words.contains(transformed) && visited.find(transformed) == visited.end()) {
                    pred[transformed] = current;
                    visited.insert(transformed);
                    visit.push(transformed);
                    if (transformed == end) {
                        std::vector<std::string> path;
                        for (std::string at = end; at != start; at = pred[at]) {
                            path.push_back(at);
                        }
                        path.push_back(start);
                        std::reverse(path.begin(), path.end());
                        return path;
                    }
                }
            }
        }
    }

    return {};
}

// You should not need to change this function
template <unsigned int H1, unsigned int H2, unsigned int EvictThreshold>
void loadWordsIntoTable(std::istream& istream,
                        BaseWordSet<H1, H2, EvictThreshold>& words) {
    std::string word;

    while (istream >> word) {
        words.insert(word);
    }
}

#endif
