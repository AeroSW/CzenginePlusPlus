#pragma once

namespace CzaraEngine {
    // https://stackoverflow.com/a/45507610/7044902
    template<typename T>
    bool isWeakPtrUnitialized(std::weak_ptr<T> const& weak) {
        using wt = std::weak_ptr<T>;
        return !weak.owner_before(wt{}) && !wt{}.owner_before(weak);
    }
}
