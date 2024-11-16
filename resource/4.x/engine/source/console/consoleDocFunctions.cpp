/// ----------------------------------------------------------------
/// Utility functions for dealing with documentation strings.
/// these are pulled from local namespace in consoleInternal
/// to provide access for other documentation consumers.
///
/// #including this will add these functions to any file's
/// local namespace.
///
/// Note: should be able to remove these from consoleInternal
/// and include this file there so the functions only need
/// updating in once place. 
/// ----------------------------------------------------------------
#include "console/consoleInternal.h"
#include <string>

using namespace std;

namespace {
    /*
    void ReplaceString(std::string& subject, const std::string& search,
        const std::string& replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
    }
    */

    /// Scan the given usage string for an argument list description.  With the
    /// old console macros, these were usually included as the first part of the
    /// usage string.
    bool sFindArgumentListSubstring(const char* usage, const char*& start, const char*& end)
    {
        if (!usage)
            return false;

        const char* ptr = usage;
        while (*ptr && *ptr != '(' && *ptr != '\n') // Only scan first line of usage string.
        {
            // Stop on the first alphanumeric character as we expect
            // argument lists to precede descriptions.
            if (dIsalnum(*ptr))
                return false;

            ptr++;
        }

        if (*ptr != '(')
            return false;

        start = ptr;
        ptr++;

        bool inString = false;
        U32 nestingCount = 0;

        while (*ptr && (*ptr != ')' || nestingCount > 0 || inString))
        {
            if (*ptr == '(')
                nestingCount++;
            else if (*ptr == ')')
                nestingCount--;
            else if (*ptr == '"')
                inString = !inString;
            else if (*ptr == '\\' && ptr[1] == '"')
                ptr++;
            ptr++;
        }

        if (*ptr)
            ptr++;
        end = ptr;

        return true;
    }

    ///
    void sParseList(const char* str, Vector< String >& outList)
    {
        // Skip the initial '( '.

        const char* ptr = str;
        while (*ptr && dIsspace(*ptr))
            ptr++;

        if (*ptr == '(')
        {
            ptr++;
            while (*ptr && dIsspace(*ptr))
                ptr++;
        }

        // Parse out list items.

        while (*ptr && *ptr != ')')
        {
            // Find end of element.

            const char* start = ptr;

            bool inString = false;
            U32 nestingCount = 0;

            while (*ptr && ((*ptr != ')' && *ptr != ',') || nestingCount > 0 || inString))
            {
                if (*ptr == '(')
                    nestingCount++;
                else if (*ptr == ')')
                    nestingCount--;
                else if (*ptr == '"')
                    inString = !inString;
                else if (*ptr == '\\' && ptr[1] == '"')
                    ptr++;
                ptr++;
            }

            // Backtrack to remove trailing whitespace.

            const char* end = ptr;
            if (*end == ',' || *end == ')')
                end--;
            while (end > start && dIsspace(*end))
                end--;
            if (*end)
                end++;

            // Add to list.

            if (start != end)
                outList.push_back(String(start, end - start));

            // Skip comma and whitespace.

            if (*ptr == ',')
                ptr++;
            while (*ptr && dIsspace(*ptr))
                ptr++;
        }
    }

    ///
    void sGetArgNameAndType(const String& str, String& outType, String& outName)
    {
        if (!str.length())
        {
            outType = String::EmptyString;
            outName = String::EmptyString;
            return;
        }

        // Find first non-ID character from right.

        S32 index = str.length() - 1;
        while (index >= 0 && (dIsalnum(str[index]) || str[index] == '_'))
            index--;

        const U32 nameStartIndex = index + 1;

        // Find end of type name by skipping rightmost whitespace inwards.

        while (index >= 0 && dIsspace(str[index]))
            index--;

        //

        outName = String(&((const char*)str)[nameStartIndex]);
        outType = String(str, index + 1);
    }

    /// Return the type name to show in documentation for the given C++ type.
    const char* sGetDocTypeString(const char* nativeType)
    {
        if (dStrncmp(nativeType, "const ", 6) == 0)
            nativeType += 6;

        if (dStrcmp(nativeType, "char*") == 0 || dStrcmp(nativeType, "char *") == 0)
            return "string";
        else if (dStrcmp(nativeType, "S32") == 0 || dStrcmp(nativeType, "U32") == 0)
            return "int";
        else if (dStrcmp(nativeType, "F32") == 0)
            return "float";

        const U32 length = dStrlen(nativeType);
        if (nativeType[length - 1] == '&' || nativeType[length - 1] == '*')
            return StringTable->insertn(nativeType, length - 1);

        return nativeType;
    }

}