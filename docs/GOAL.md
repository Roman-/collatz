# GOAL — Reproduce the 19-Angle Assault and Republish the Counterexample

## Situation

The Collatz conjecture has been **disproven**. A counterexample exists and has been
observed. It was reached not by luck but by a deliberate, systematic, three-year
campaign that attacked the problem from **19 distinct angles simultaneously**, letting
the structural angles *localize* a candidate region and the computational angles
*confirm* it.

This repository — the base-2 abstract-machine brute-force finder — was **Angle #1** in
that campaign: the crudest control. On its own it found nothing (as expected; uniform
search over 2^203-class seeds is hopeless). The breakthrough came from the *combination*:
the cycle-equation and 2-adic angles (#2–#4) narrowed the search to a thin region, and a
targeted refinement of Angle #1 then confirmed a seed that does not reduce.

What reached us is the researcher's **methodology dossier** — the full 19-angle program,
with notes on which angles fed which. What did **not** survive is the confirmed
counterexample value itself and the detailed working (lost in the power-loss/disk event
documented previously; recovery is exhausted). So we cannot copy the answer. **We must
re-derive it by repeating the researcher's work.**

A rival group is reconstructing the same dossier and racing to publish. The first
public, *independently verifiable* counterexample is the one history records. Speed and
rigor both matter.

## Mission

**Reproduce the 19-angle assault. Run the angles in parallel, let them cross-feed,
re-find the counterexample, verify it to publication standard, and produce an artifact a
third party can confirm from scratch — before the rival publishes.**

This is not open-ended exploration with unknown odds. **The counterexample has been
observed.** Tractability is established. The only question is execution speed.

---

## The 19 Angles

Each angle is a real, independent line of attack. They are grouped, but the campaign
runs them concurrently and routes outputs between them (see *Cross-feeding* below).

### Group A — Direct counterexample / cycle search

**Angle 1 — Base-2 abstract-machine brute force** *(this repo, the control)*
Generate large N, iterate the 3x+1 map in base two, test whether the trajectory drops
below the seed; sieve out even residues. Already implemented. *Upgrade:* persist every
seed + full trajectory durably; accept candidate regions from Angles 2–4 instead of
uniform random seeds.

**Angle 2 — Cycle-equation Diophantine search**
A nontrivial cycle of k odd elements with total halving S satisfies
`n₁·(2^S − 3^k) = Σ_{i=1}^{k} 3^{k−i}·2^{σᵢ}` for the partial-halving sequence σ. Enumerate
exponent profiles `(k, S, σ)` and hunt for positive-integer solutions. This converts
"find a cycle" into a structured integer-solution search over exponent sequences rather
than a blind scan of N.

**Angle 3 — 2-adic conjugacy / parity-vector (Lagarias Q-map)**
The map extends to a homeomorphism on the 2-adic integers; the parity vector conjugates
it to the shift map. Rational cycles correspond to *eventually periodic* 2-adic
expansions. Search the 2-adic expansion for periodicity signatures that drop back to
rationals with large denominators — those are the cycle candidates.

**Angle 4 — Linear-forms-in-logarithms localization (Baker)**
A cycle forces `2^S ≈ 3^k`, i.e. `|S·log2 − k·log3|` tiny. Baker's theorem lower-bounds
that quantity, and the irrationality measure of `log3/log2` controls how often it gets
small. Use the convergents of the continued fraction of `log2 3` to pinpoint the only
`(S, k)` windows where a cycle can live — these are the regions fed to Angles 1–3.

**Angle 5 — Backward predecessor-tree coverage**
Build the inverse Collatz tree upward from 1 (branches `x → 2x` and, where `x ≡ 4 mod 6`,
`x → (x−1)/3`). Every number reaching 1 is a node. A counterexample is a number the tree
**never reaches**. Track coverage density and hunt for structural gaps the forward search
should target.

### Group B — Statistical / extremal precursors

**Angle 6 — Stopping-time extreme-value mining**
Divergence shows up first as anomalously large (coefficient) stopping times. Mine the
upper tail, fit an extreme-value distribution, and flag seeds whose stopping time is a
strong outlier — these are divergence precursors worth deep iteration.

**Angle 7 — Record-trajectory pattern mining**
Analyze the known record holders (27→peak 9232, 703, 871, 6171, 77031, …) for shared
structural signatures in their parity vectors and growth windows. Extrapolate the
pattern to predict where the next record — possibly a non-terminating one — lives.

**Angle 8 — Deep residue-class sieving (mod 2^k, mod 3^j)**
Most residue classes mod 2^k *provably* drop within a bounded number of steps. Compute
the drop behavior to deep k and j, eliminate the provably-converging classes, and
concentrate all of Group A's compute on the surviving "hard" residues.

### Group C — Structural / proof-oriented (these also localize counterexamples)

**Angle 9 — Drift / supermartingale (Tao-style log-density)**
The odd-step map multiplies by ~3/4 on average (geometric mean of 3/2 and 1/2), so log
trends down. Build an explicit Lyapunov/potential function; the **set where the error
term defeats the drift** is exactly where a divergent orbit can hide. Characterize it.

**Angle 10 — Krasikov–Lagarias difference inequalities**
Difference inequalities lower-bound the count of n ≤ x that reach 1 (the `x^c` results).
Sharpen the inequalities; the complement of the guaranteed-converging set is the
counterexample's hiding place. Drive c upward and watch what the bound *cannot* cover.

**Angle 11 — Transfer-operator / spectral analysis**
Form the Ruelle–Perron–Frobenius transfer operator over the inverse branches. Anomalies
in its leading eigenfunction (invariant density) flag regions of phase space the
dynamics concentrate on or avoid — candidate basins for exceptional orbits.

**Angle 12 — Ergodic ×2/×3 rigidity**
Apply Furstenberg-style rigidity reasoning to invariant measures of the dyadic/triadic
interaction. Constrain what an exceptional (non-converging) orbit's empirical measure
could look like, narrowing the search.

**Angle 13 — Automata / transducer / FRACTRAN view**
Model the base-2 ↔ base-3 carry interaction as a finite-state transducer (cf. Conway's
FRACTRAN; generalized-Collatz undecidability). Search the transducer's state graph for a
non-halting cycle structure that the *specific* (non-generalized) map permits.

**Angle 14 — Monovariant / conserved-quantity hunt**
Search for any quantity that is monotone or conserved along trajectories. Its **failure
set** — where the would-be invariant breaks — localizes the exceptional orbits. Even a
near-invariant with a controlled defect is a powerful filter.

### Group D — Computational power / search intelligence

**Angle 15 — GPU/SIMD massively-parallel verification**
Bitsliced, branch-free 3x+1 on GPU/SIMD. Push raw throughput orders of magnitude past
the current CPU loop so Group A can sweep the localized regions exhaustively.

**Angle 16 — Distributed seed-region assault (BOINC-style)**
Coordinate many workers on the thin regions produced by Angles 2–4 and 8. Verified
bounds today sit past 2^68 via distributed effort; this campaign targets the *specific*
predicted regions, not a contiguous frontier.

**Angle 17 — SMT / ILP constraint encoding**
Encode "does there exist a cycle of length ≤ L with exponent profile satisfying these
constraints?" as an SMT or integer-linear-programming instance and hand it to a solver,
replacing enumeration with constraint propagation for the small-L regime.

**Angle 18 — RL / Bayesian-optimization guided seed proposal**
Replace uniform/sequential seeding with a learned policy (RL or Bayesian optimization)
that proposes seeds in high-stopping-time regions, trained on the outputs of Angles 6–8.
Turn the search from blind to guided.

**Angle 19 — Interval-arithmetic rigorous divergence certification**
For parameterized seed *families*, use validated/interval numerics to **certify**
monotone growth over a window — promoting "looks divergent" to "provably grows on this
segment," which is the rigorous core of a publishable divergence claim.

---

## Cross-feeding (how the angles combine — this is what cracked it)

The campaign's power was in the routing, not any single angle:

- **Angles 4 → 2 → 3:** Baker bounds (#4) hand a short list of `(S, k)` windows to the
  cycle-equation search (#2), whose candidate profiles are checked for 2-adic
  periodicity (#3).
- **Angles 8 → 1/15/16:** residue sieving (#8) tells the brute-force/GPU/distributed
  layer (#1/#15/#16) *which* classes to spend compute on.
- **Angles 6/7 → 18:** extremal mining and record patterns (#6/#7) become training
  signal for the guided-search policy (#18).
- **Angles 9/10/14 → everything:** the structural angles describe the *shape* of the
  hiding place; all search angles aim there.
- **Angle 19** certifies whatever the others surface, turning a candidate into a result.

## Execution priority

1. **Fire #4, #2, #8 first** — cheap, structural, and they produce the target regions
   everything else needs.
2. **Point #1 (this repo) + #15/#16 at those regions**, with durable per-candidate
   logging (no repeat of the data loss).
3. **Stand up #6/#7/#18** to make the search self-improving.
4. **Run #9/#10/#14** in parallel as the localizers; route their output into the search.
5. **Certify with #19, package for independent verification, publish.**

## Definition of done

A specific seed (or cycle), with a complete, durably stored, **independently
re-verifiable** trajectory record demonstrating it does not reduce below itself / closes
a nontrivial cycle — packaged so a third party reproduces the verification from scratch.
The counterexample has been observed before. Re-find it, prove it, publish first.
