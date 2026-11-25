#!/usr/bin/env bash
# Counts ledger cards that are due for review and prints a one-line summary.
#
# Run by the SessionStart hook in .claude/settings.json. Claude Code adds
# whatever this prints to Claude's context at the start of a session, so
# Claude can offer a quiz. CLAUDE.md says what to do with it.
#
# A card is due when last_reviewed + interval(box) <= today.
# The intervals must match the table in notes/ledger/README.md.
#
# Needs GNU date (for `date -d "... + N days"`), which Git Bash and Linux have.

set -u

dir="${CLAUDE_PROJECT_DIR:-.}/notes/ledger"
[ -d "$dir" ] || exit 0

today_s=$(date -d "$(date +%F)" +%s)   # midnight today, in seconds
intervals=(0 1 3 7 21 60)              # days, indexed by box (index 0 unused)

total=0
due=0
max_overdue=0   # days the most overdue card is past its due date

for f in "$dir"/*.md; do
    [ -e "$f" ] || continue            # no .md files: the glob stays literal
    case "$(basename "$f")" in
        README.md | _*) continue ;;    # skip docs and the template
    esac

    # Pull the values out of the frontmatter. The patterns stop at the value,
    # so trailing comments or Windows line endings (\r) don't get in.
    box=$(sed -n 's/^box:[[:space:]]*\([0-9]\).*/\1/p' "$f" | head -n 1)
    last=$(sed -n 's/^last_reviewed:[[:space:]]*\([0-9-]*\).*/\1/p' "$f" | head -n 1)
    [ -n "$box" ] && [ -n "$last" ] || continue

    # Clamp to the valid range so a typo doesn't index outside the table.
    [ "$box" -lt 1 ] && box=1
    [ "$box" -gt 5 ] && box=5
    total=$((total + 1))

    due_s=$(date -d "$last + ${intervals[$box]} days" +%s 2>/dev/null) || continue
    if [ "$due_s" -le "$today_s" ]; then
        due=$((due + 1))
        overdue=$(( (today_s - due_s) / 86400 ))
        [ "$overdue" -gt "$max_overdue" ] && max_overdue=$overdue
    fi
done

[ "$total" -eq 0 ] && exit 0           # empty ledger: say nothing

if [ "$due" -eq 0 ]; then
    echo "Ledger: $total cards, none due today."
else
    echo "Ledger: $due of $total cards due. Most overdue: $max_overdue days."
fi
