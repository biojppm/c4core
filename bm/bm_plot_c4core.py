import sys
import os
import re

thisdir = os.path.dirname(os.path.abspath(__file__))
moddir = os.path.abspath(f"{thisdir}/../cmake/bm-xp")
sys.path.insert(0, moddir)
import bm_plot as bm
from bm_util import first

from dataclasses import dataclass
import prettytable


def get_function_benchmark(function_name, run: bm.BenchmarkRun):
    for rbm in run.entries:
        if rbm.meta.function == function_name:
            return rbm
    raise Exception(f"function not found: {function_name}. Existing: {[rbm.meta.function for rbm in run.entries]}")


@dataclass
class CharconvMeta:  # also for atox
    title: str
    subject: str
    function: str
    data_type: bm.FundamentalTypes

    @classmethod
    def make(cls, bm_title: str):
        # eg:
        #   xtoa_c4_write_dec<uint8_t>
        #   xtoa_c4_utoa<uint8_t>
        #   xtoa_c4_xtoa<uint8_t>
        #   xtoa_c4_to_chars<uint8_t>
        #   xtoa_std_to_chars<uint8_t>
        #   xtoa_std_to_string<uint8_t>
        #   xtoa_sprintf<uint8_t>
        #   xtoa_sstream_reuse<uint8_t>
        #   xtoa_sstream<uint8_t>
        rx = re.compile(r'(atox|xtoa|xtoahex|xtoaoct|xtoabin)_(.*?)<(u?int\d+_t|float|double)>')
        if not rx.fullmatch(bm_title):
            raise Exception(f"cannot understand bm title: {bm_title}")
        subject = rx.sub(r'\1', bm_title)
        function = rx.sub(r'\2', bm_title)
        data_type = rx.sub(r'\3', bm_title)
        return cls(
            title=bm_title,
            subject=subject,
            function=function.replace("c4_", "c4::").replace("std_", "std::"),
            data_type=bm.FundamentalTypes.make(data_type)
        )

    @property
    def shortname(self):
        return self.function

    @property
    def shortparams(self):
        return str(self.data_type.short)

    @property
    def shorttitle(self):
        return f"{self.shortname}<{self.shortparams}>"


@dataclass
class CharconvThreadsMeta:
    function: str
    num_threads: int

    @classmethod
    def make(cls, bm_title: str):
        # eg:
        #   c4_itoa/real_time/threads:4
        rx = re.compile(r'(.*?)/real_time/threads:(\d+)')
        if not rx.fullmatch(bm_title):
            raise Exception(f"cannot understand bm title: {bm_title}")
        function = rx.sub(r'\1', bm_title)
        num_threads = int(rx.sub(r'\2', bm_title))
        return cls(
            function=function.replace("c4_", "c4::").replace("std_", "std::"),
            num_threads=num_threads
        )

    def checkbox_groups(self):
        return {}

    @property
    def shortname(self):
        return self.function

    @property
    def shorttitle(self):
        return self.shortname


def plot_charconv_bars(bm_panel: bm.BenchmarkPanel, getref,
                       panel_title_human: str,
                       outputfile_prefix: str):
    assert os.path.isabs(outputfile_prefix), outputfile_prefix
    # make a comparison table
    anchor = lambda run: f"{os.path.basename(outputfile_prefix)}-{first(run.meta).data_type}"
    anchorlink = lambda run: f"<pre><a href='#{anchor(run)}'>{first(run.meta).data_type}</a></pre>"
    with open(f"{outputfile_prefix}.txt", "w") as tablefile:
        with open(f"{outputfile_prefix}.md", "w") as mdfile:
            print(f"## {panel_title_human}\n\n<p>Data type benchmark results:</p>\n<ul>\n",
                  "\n".join([f"  <li>{anchorlink(run)}</li>" for run in bm_panel.runs]),
                  "</ul>\n\n", file=mdfile)
            for run in bm_panel.runs:
                data_type = first(run.meta).data_type
                tabletitle = f"{outputfile_prefix}-{data_type.short}"
                table = prettytable.PrettyTable(title=f"{panel_title_human}: {data_type}")
                table.add_column("function", [m.shorttitle for m in run.meta], align="l")
                for prop in ("mega_bytes_per_second", "cpu_time_ms"):
                    ref = getref(run)
                    bar_values = list(run.extract_plot_series(prop))
                    bar_values_rel = list(run.extract_plot_series(prop, relative_to_entry=ref))
                    bar_values_pc = list(run.extract_plot_series(prop, percent_of_entry=ref))
                    pd = bm_panel.first_run.property_plot_data(prop)
                    hns = pd.human_name_short
                    table.add_column(hns, [f"{v_:7.2f}" for v_ in bar_values], align="r")
                    hns = hns.replace(" (ms)", "")
                    table.add_column(f"{hns}(x)", [f"{v_:5.2f}x" for v_ in bar_values_rel], align="r")
                    table.add_column(f"{hns}(%)", [f"{v_:7.2f}%" for v_ in bar_values_pc], align="r")
                print(table, "\n\n")
                print(table, "\n\n", file=tablefile)
                pfx_bps = f"{os.path.basename(outputfile_prefix)}-mega_bytes_per_second-{data_type.short}"
                pfx_cpu = f"{os.path.basename(outputfile_prefix)}-cpu_time_ms-{data_type.short}"
                print(f"""
<br/>
<br/>

---

<a id="{anchor(run)}"/>

### {panel_title_human}: `{data_type}`

* Interactive html graphs for `{data_type}`:
  * [MB/s](./{pfx_bps}.html)
  * [CPU time](./{pfx_cpu}.html)

[![{data_type}: MB/s](./{pfx_bps}.png)](./{pfx_bps}.png)
[![{data_type}: CPU time](./{pfx_cpu}.png)](./{pfx_cpu}.png)

```
{table}
```
""", file=mdfile)
    # make plots
    for prop in ("mega_bytes_per_second", "cpu_time_ms"):
        ps, ps_ = [], []
        pd = bm_panel.first_run.property_plot_data(prop)
        bar_label = f"{pd.human_name_short}{pd.qty_type.comment}"
        outfilename = f"{outputfile_prefix}-{prop}"
        for run in bm_panel.runs:
            data_type = first(run.meta).data_type
            bar_names = [m.shorttitle for m in run.meta]
            bar_values = list(run.extract_plot_series(prop))
            runtitle = f"{outfilename}-{data_type.short}"
            # to save each bokeh plot separately and also
            # a grid plot with all of them, we have to plot
            # twice because bokeh does not allow saving twice
            # the same plot from multiple pictures.
            plotit = lambda: bm.plot_benchmark_run_as_bars(run, title=f"{panel_title_human}: {data_type}\n{bar_label}",
                                                           bar_names=bar_names, bar_values=bar_values, bar_label=bar_label)
            # make one plot to save:
            p, p_ = plotit()
            bm._bokeh_save_html(f"{runtitle}.html", p)
            bm._plt_save_png(f"{runtitle}.png")
            bm._plt_clear()
            # and another to gather:
            p, p_ = plotit()
            ps.append(p)
            ps_.append(p_)
            bm._plt_clear()
        bm.bokeh_plot_many(ps, f"{outfilename}.html")


def plot_itoa_threads_(bm_panel: bm.BenchmarkPanel, getref,
                       panel_title_human: str,
                       outputfile_prefix: str):
    assert os.path.isabs(outputfile_prefix), outputfile_prefix
    orig = lambda yprop, **kw: lambda run: list(run.extract_plot_series(yprop, **kw))
    divnt = lambda yprop, **kw: lambda run: [v / n for v, n in run.extract_plot_series_with_threads(yprop, **kw)]
    mulnt = lambda yprop, **kw: lambda run: [v * n for v, n in run.extract_plot_series_with_threads(yprop, **kw)]
    xprop = "threads"
    xpd = bm_panel.first_run.property_plot_data(xprop)
    xlabel = f"{xpd.human_name_short}"
    for yprop, ylog, yget in (
            #("mega_items_per_second", False, orig),
            ("mega_bytes_per_second", False, orig),
            #("iterations", False, divnt),
            #("real_time_ms", True, mulnt),
            ("cpu_time_ms", True, orig),):
        ypd = bm_panel.first_run.property_plot_data(yprop)
        ylabel = f"{ypd.human_name_short}{ypd.qty_type.comment}"
        p = bm.plot_benchmark_panel_as_lines(
            bm_panel, f"{panel_title_human}\n{ylabel}",
            xget=orig("threads"),
            yget=yget(yprop),
            nameget=lambda run: first(run.meta).function,
            ylog=ylog,
            xlabel=xlabel,
            ylabel=ylabel
        )
        name = f"{outputfile_prefix}-lines-{yprop}"
        # save png using matplotlib
        bm._plt_save_png(f"{name}.png")
        bm._plt_clear()
        # save html using bokeh
        bm._bokeh_save_html(f"{name}.html", p)
        #bkp.show(p)
    return p


def plot_itoa_threads(dir_: str, json_files):
    panel = bm.BenchmarkPanel(json_files, CharconvThreadsMeta)
    ref = lambda bmrun: get_function_benchmark("std::to_chars", run=bmrun)
    plot_itoa_threads_(panel, ref,
                       f"itoa benchmark: convert 2M 32b integers to string",
                       f"{dir_}/c4core-bm-charconv_threads")


def plot_charconv_xtoa(dir_: str, json_files, is_ftoa: bool):
    fcase = "ftoa" if is_ftoa else "xtoa"
    panel = bm.BenchmarkPanel(json_files, CharconvMeta)
    ref = lambda bmrun: get_function_benchmark("sprintf", run=bmrun)
    plot_charconv_bars(panel, ref,
                       f"xtoa benchmark: convert 1M numbers to strings",
                       f"{dir_}/c4core-bm-charconv-{fcase}")


def plot_charconv_atox(dir_: str, json_files, is_atof: bool):
    fcase = "atof" if is_atof else "atox"
    panel = bm.BenchmarkPanel(json_files, CharconvMeta)
    ref = lambda bmrun: get_function_benchmark("scanf", run=bmrun)
    plot_charconv_bars(panel, ref,
                       f"atox benchmark: convert 1M strings to numbers",
                       f"{dir_}/c4core-bm-charconv-{fcase}")


if __name__ == '__main__':
    args = sys.argv[1:]
    if len(args) < 2:
        raise Exception(f"usage: {sys.executable} {sys.argv[0]} <atox|xtoa|itoa_threads|format|digits> benchmarkfile.json[,benchmarkfile2.json,...]")
    cmd = args[0]
    json_files = args[1:]
    dir_ = os.path.dirname(json_files[0])
    for jf in json_files:
        print("jf:", jf, flush=True)
        assert os.path.dirname(jf) == dir_, (os.path.dirname(jf), dir_)
        assert os.path.exists(jf), jf
    if cmd == "itoa_threads":
        plot_itoa_threads(dir_, json_files)
    elif cmd == "xtoa" or cmd == "ftoa":
        plot_charconv_xtoa(dir_, json_files, (cmd == "ftoa"))
    elif cmd == "atox" or cmd == "atof":
        plot_charconv_atox(dir_, json_files, (cmd == "atof"))
    elif cmd == "format":
        raise Exception(f"not implemented: {cmd}")
    elif cmd == "digits":
        pass  # nothing to do
    else:
        raise Exception(f"not implemented: {cmd}")
